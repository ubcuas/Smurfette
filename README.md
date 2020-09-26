# Smurfette
`Smurfette` is our telemetry aggregation service.

## Connections
```
[GCOM-X]---<http/s>---[Smurfette]---<tcp>---[Skylink]
                            |
                          <tcp>
                            |
                        [Skylink]
```

## Dependencies
**Docker:**
- Docker

**Local:**
- Rust + Cargo
- Libuuas

## Installation
The image can be directly pulled from DockerHub:
```
docker pull ubcuas/smurfette:latest
```
The image can also be built locally:
```
docker build --tag ubcuas/smurfette:latest .
```
`Smurfette` can also be built locally directly:
```
cargo build --release
```

## Usage
General usage is as follows:
```
docker run -it ubcuas/smurfette:latest <GCOM_HOSTNAME> <SKYLINK_SOURCE>...
```

Full command line options are as follows:
```
smurfette 0.1.0
Telemtry to the people!

USAGE:
    smurfette <GCOM_HOSTNAME> <SKYLINK_SOURCE>...

FLAGS:
    -h, --help       Prints help information
    -V, --version    Prints version information

ARGS:
    <GCOM_HOSTNAME>        Hostname to access gcom, for example `www.gcom.ca` or `192.168.1.44`
    <SKYLINK_SOURCE>...    TCP input string for a skylink source, for example `127.0.0.1:5555`
```

If you wanted to send telemetry from a `Skylink` instance serving on port 5555 to `GCOM-X` which is running on port 8080:
```
docker run -it --init ubcuas/smurfette:latest 127.0.0.1:8080 127.0.0.1:5555
```

If you wanted to aggregate telemetry from multiple `Skylink` instances serving on ports 5555, 6666 and 7777 to `GCOM-X` which is running on port 8080:
```
docker run -it --init ubcuas/smurfette:latest 127.0.0.1:8080 127.0.0.1:5555 127.0.0.1:6666 127.0.0.1:7777
```

## Troubleshooting
Contact `Eric Mikulin`
