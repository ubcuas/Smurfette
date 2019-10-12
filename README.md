[![License: MIT](https://img.shields.io/github/license/vintasoftware/django-react-boilerplate.svg)](LICENSE.txt)

# UBC UAS SMURFETTE

## About
This project is UBC UAS' telemetry aggregation service.

See the full documentation on UAS confluence [here](http://confluence.ubcuas.com/display/GCOM/Software+Systems+Documentation+2019) (Note: You have to be using ubcsecure or the UBC VPN in order to access.)

## Setup
**Using docker container environment:**

- First, install `docker` and `docker-compose`

```shell
    $ git submodule update --init --recursive
    $ sudo docker build . -t smurfette
```

## Running - docker
**Start the server using docker-compose**

```shell
    $ sudo docker run -it smurfette sh
    $ ./smurfette
```

Note, you need to run bash and then the program first, since the docker container doesn't handle signal interrupts correctly.