docker-machine create --driver virtualbox PingMachine;
eval $(docker-machine env PingMachine);
docker build -t ping-build .;
docker stop ping-run;
docker rm ping-run;
docker run --name t3r -it ping-build;
