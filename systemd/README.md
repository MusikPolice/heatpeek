# Managing Promethius with Docker and Systemd
One way to manage Promethius is to run it in a Docker container. If hosted on a Linux server, the container can be made to start at boot via Systemd. 

To configure Promethius in this way, copy the files in this directory to the corresponding directories on your Linux server. Consider this directory to be the root directory on your server, meaning that everything in the `lib/systemd/system` directory of this repository should be copied to the `/lib/systemd/system` directory of your Linux server.

## Starting Prometheus
Once the `prometheus.service` and `prometheus.yml` files have been copied to their respective homes on your server, run `sudo systemctl start prometheus.service`, and Prometheus should start on port 9090 of your server.

Once the service has been started, the Prometheus docker container should be visible in `docker ps`:
```
user@SERVER $ docker ps
CONTAINER ID        IMAGE                COMMAND                  CREATED             STATUS              PORTS                    NAMES
32b6b0d0ff37        prom/prometheus      "/bin/prometheus -..."   17 seconds ago      Up 6 seconds        0.0.0.0:9090->9090/tcp   prometheus
``` 

## Configuring Prometheus
Prometheus is configured via the `prometheus.yml` file in the `/etc/prometheus` directory. Detailed configuration instructions can be found in the [Prometheus Documentation](https://prometheus.io/docs/prometheus/latest/configuration/configuration/)
