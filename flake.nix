{
	description = "C++ Development Environment";

	inputs = {
		nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
		flake-utils.url = "github:numtide/flake-utils";
	};

	outputs = { self, nixpkgs, flake-utils }:
		flake-utils.lib.eachDefaultSystem (system:
			let
				pkgs = import nixpkgs { system = system; };
			in {
				devShells.default =
					pkgs.mkShell {
						nativeBuildInputs = with pkgs.buildPackages; [
							gcc
							cmake
							gnumake
							openssl
							curl
							fmt
							dpp
						];
					};

packages.default = pkgs.stdenv.mkDerivation {
	name    = "verifybot";
	version = "0.1.0";
	src     = ./.;

	buildPhase = ''
		mkdir build
		cmake -B build -S "$src" \
			-DCMAKE_FIND_USE_SYSTEM_PACKAGE_REGISTRY=OFF \
			-DCMAKE_FIND_USE_PACKAGE_REGISTRY=OFF \
			-DCMAKE_EXPORT_NO_PACKAGE_REGISTRY=ON \
			-DCMAKE_BUILD_TYPE=Release
		cmake --build build
	'';

	nativeBuildInputs = with pkgs.buildPackages; [
		gcc
		git
		cmake
		gnumake
		openssl
		curl
		libcpr
		fmt
		dpp
	];

	installPhase = ''
		mkdir -p $out/bin
		cp build/VerifyBot $out/bin/
	'';
};


			}
		);
}
