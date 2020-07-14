use clap::{load_yaml, App};
use crossbeam::crossbeam_channel;
use reqwest;
use ruuas::telem;
use std::io::Read;
use std::net::TcpStream;
use std::sync::{Arc, Mutex};
use std::thread;
use std::time;

mod ttimer;

const BUF_SIZE: usize = 256;

struct SkylinkListenerArgs {
    connection_string: String,
    telem_sender: Arc<Mutex<telem::TelemetryArgs>>,
}

fn skylink_listener(args: SkylinkListenerArgs) -> std::io::Result<()> {
    println!(
        "Connecting to skylink instance: {}",
        &args.connection_string
    );
    let mut skylink_src = TcpStream::connect(&args.connection_string)?;
    println!("Connected!: {}", &args.connection_string);

    loop {
        let mut buf: [u8; BUF_SIZE] = [0; BUF_SIZE];
        match skylink_src.read(&mut buf) {
            Ok(bytes_read) => match telem::deserialize_telem_msg(&buf[..bytes_read]) {
                Ok(telem_msg) => {
                    let mut telem_sender = &mut args.telem_sender.lock().unwrap();
                    telem::write_msg_to_args(&mut telem_sender, telem_msg);
                }
                Err(e) => println!("({}) Failed to parse telem: {}", &args.connection_string, e),
            },
            Err(e) => println!("({}) Failed to get bytes: {}", &args.connection_string, e),
        }
    }
}

fn main() -> std::io::Result<()> {
    let arg_yml = load_yaml!("cli.yml");
    let matches = App::from(arg_yml).get_matches();

    let gcom_hostname = matches.value_of_lossy("gcomhostname").unwrap();
    let skylink_sources = matches.values_of_lossy("skylinksource").unwrap();

    let mut threads = vec![];
    let mut telem_senders = vec![];

    for source in skylink_sources {
        let telem_sender = Arc::new(Mutex::new(telem::TelemetryArgs::default()));
        telem_senders.push(telem_sender.clone());

        threads.push(thread::spawn(move || loop {
            let args = SkylinkListenerArgs {
                connection_string: source.clone(),
                telem_sender: telem_sender.clone(),
            };

            match skylink_listener(args) {
                Ok(_) => {
                    println!("Skylink listener exited, exiting...");
                    break;
                }
                Err(e) => {
                    println!("Skylink listener error: {}", e);
                    println!("Skylink listener restarting");
                    continue;
                }
            }
        }));
    }

    let client = reqwest::blocking::Client::new();
    let mut timer = ttimer::TickTimer::new(1000);
    loop {
        let mut latest_telem: telem::TelemetryArgs = telem::TelemetryArgs::default();
        for tsend in &telem_senders {
            let telem = tsend.lock().unwrap();
            if telem.timestamp_pixhawk_ms > latest_telem.timestamp_pixhawk_ms {
                latest_telem = telem.clone();
            }
        }

        let post_url = format!("http://{}/api/interop/telemetry", &gcom_hostname);
        let post_data = telem::serialize_telem_msg(telem::new_telem_msg(latest_telem.clone()));

        match client.post(&post_url).body(post_data).send() {
            Ok(res) => println!("{:?}", res),
            Err(e) => println!("Unable to POST telemetry: {}", e),
        };

        &timer.next();
    }

    for thread in threads {
        thread.join().unwrap();
    }

    return Ok(());
}
