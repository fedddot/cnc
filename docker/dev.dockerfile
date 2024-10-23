FROM golang:1.23

# files should be mapped into this directory when running the container
WORKDIR /usr/src/app

CMD ["/bin/bash"]
