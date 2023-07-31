{ compiler ? "",
  nixpkgs ? (import <nixpkgs> {}).fetchFromGitHub {
    owner  = "NixOS";
    repo   = "nixpkgs";
    rev    = "31cd1b4afbaf0b1e81272ee9c31d1ab606503aed";
    sha256 = "1hd48vkdrj8rgihrkjfhr3amqlf3phjshqyylslb836n6vlp18pp";
  }}:

with import nixpkgs {};

let
  # For the documentation tools we use an older Nixpkgs since the
  # newer versions seem to be not working great...
  old-nixpkgs-src = fetchFromGitHub {
                      owner  = "NixOS";
                      repo   = "nixpkgs";
                      rev    = "d0d905668c010b65795b57afdf7f0360aac6245b";
                      sha256 = "1kqxfmsik1s1jsmim20n5l4kq6wq8743h5h17igfxxbbwwqry88l";
                    };
  old-nixpkgs     = import old-nixpkgs-src {};
  docs            = import ./nix/docs.nix { nixpkgs = old-nixpkgs-src; };
  compiler-pkg    = if compiler != ""
                    then pkgs.${compiler}
                    else stdenv.cc;
  the-stdenv      = if compiler-pkg.isClang
                    then clangStdenv
                    else stdenv;

in

the-stdenv.mkDerivation rec {
  name = "zug-env";
  buildInputs = [
    cmake
    boost
    ccache
    old-nixpkgs.doxygen
    (old-nixpkgs.python.withPackages (ps: [
      ps.sphinx
      docs.breathe
      docs.recommonmark
    ]))
  ];
  shellHook = ''
    export ZUG_ROOT=`dirname ${toString ./shell.nix}`
    addToSearchPath PATH "$ZUG_ROOT/build"
    addToSearchPath PATH "$ZUG_ROOT/build/example"
    addToSearchPath PATH "$ZUG_ROOT/build/test"
  '';
}
