FROM gcc:13.2.0 AS cpr
RUN apt-get update \
 && apt-get -y install cmake git curl \
 && apt-get clean \
 && rm -rf /var/lib/apt/lists/*
WORKDIR /usr/src
RUN git clone https://github.com/libcpr/cpr.git cpr \
 && cd cpr \
 && git checkout 2553fc41450301cd09a9271c8d2c3e0cf3546b73 \
 && cmake \
          -DCPR_BUILD_TESTS=OFF \
	  -DCPR_USE_SYSTEM_CURL=ON \
          -B build \
 && cmake --build build -j$(nproc) \
 && cmake --install build

FROM cpr AS dpp
RUN git clone https://github.com/brainboxdotcc/DPP.git dpp \
 && cd dpp \
 && git checkout 63ac3fed8bdd92e3122c6079b944f70fc9546dc2 \
 && cmake \
          -DDPP_BUILD_TEST=OFF \
          -DBUILD_VOICE_SUPPORT=OFF \
	  -B build \
 && cmake --build build -j$(nproc) \
 && cmake --install build

FROM dpp
RUN useradd -m -s /bin/bash user \
 && mkdir /home/user/verifybot
COPY ./config.json /home/user/verifybot/
RUN chown -R user: /home/user/verifybot
COPY . /usr/src/verifybot
WORKDIR /usr/src/verifybot
RUN cmake -B build \
 && cmake --build build -j$(nproc) \
 && cmake --install build
USER user
WORKDIR /home/user/verifybot
CMD /usr/local/bin/VerifyBot
