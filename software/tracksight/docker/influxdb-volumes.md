### InfluxDB data persistence

InfluxDB data is stored in named Docker volumes (`influx-data`, `influx-config`) declared in each config under `docker/configs/`. These volumes live independently of the container.

- Safe (data preserved): `docker compose ... down`, `docker compose ... up --build`, removing/recreating containers, upgrading the image.
- Destructive (wipes data): `docker compose ... down -v`, `docker volume rm tracksight-docker_influx-data`, or pruning all volumes.

To inspect/back up the data volume:
```
docker volume ls | grep influx
docker run --rm -v tracksight-docker_influx-data:/data -v $PWD:/backup alpine \
  tar czf /backup/influx-data.tgz -C /data .
```
(The volume name prefix matches the `name:` field in your chosen config — `tracksight-docker`, `tracksight-wireless`, or `tracksight-db`.)

To clean volumes:
```
docker volume rm <volume name>
```
