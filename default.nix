with import <nixpkgs> {};

stdenv.mkDerivation rec {
  name = "zug-git";
  version = "git";
  src = builtins.filterSource (path: type:
            baseNameOf path != ".git" &&
            baseNameOf path != "build" &&
            baseNameOf path != "_build" &&
            baseNameOf path != "reports" &&
            baseNameOf path != "tools")
            ./.;
  buildInputs = [
    cmake
    boost
  ];
  cmakeFlags = [
    "-Dzug_BUILD_TESTS=OFF"
    "-Dzug_BUILD_EXAMPLES=OFF"
  ];
  meta = with lib; {
    homepage    = "https://github.com/arximboldi/zug";
    description = "library for functional interactive c++ programs";
    license     = licenses.lgpl3Plus;
  };
}
