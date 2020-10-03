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
docker run -it --init ubcuas/smurfette:latest <GCOM_HOSTNAME> <SKYLINK_SOURCE>...
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
docker run --rm -it --init --network=gcom-x_uasnet --name=smurfette ubcuas/smurfette:latest gcomx-backend:8080 skylink:5555
```

If you wanted to aggregate telemetry from multiple `Skylink` instances serving on ports 5555, 6666 and 7777 to `GCOM-X` which is running on port 8080:
```
docker run --rm -it --init --network=gcom-x_uasnet --name=smurfette ubcuas/smurfette:latest gcomx-backend:8080 skylink1:5555 skylink2:6666 skylink3:7777
```


## Troubleshooting
----
`docker: Error response from daemon: network gcom-x_uasnet not found.`
> You need to create the network that the containers connect to. Starting up `gcom-x` will create the network.
> It can also manually be created using the command `docker network create gcom-x_uasnet`.

----
`Cannot connect to the Docker daemon at unix:///var/run/docker.sock. Is the docker daemon running?` or similar.
> You need to run the `docker` commands as root. Use sudo: `sudo docker <command>`. Or add yourself to the docker group.

----
`Anything Else`
> Contact `Eric Mikulin`

