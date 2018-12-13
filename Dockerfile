FROM debian

RUN apt-get update \
	&& apt-get install git -y \
	&& apt-get install emacs -y \
	&& apt-get install -y make \
	&& apt-get install -y clang
