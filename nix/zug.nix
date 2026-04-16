{
  lib,
  stdenv,
  fetchFromGitHub,
  cmake,
  ninja,
  boost,
  catch2_3,
  sources ? ../.,
  withTests ? false,
  withExamples ? false,
  withDocs ? false,
  withDebug ? false,
  withCoverage ? false,
}:

let
  docs = import ./docs.nix {
    inherit
      lib
      stdenv
      fetchFromGitHub
      ;
  };
in
stdenv.mkDerivation {
  name = builtins.concatStringsSep "-" (
    [ "zug" ]
    ++ lib.optionals withDebug [ "debug" ]
    ++ lib.optionals withTests [ "tests" ]
    ++ lib.optionals withExamples [ "examples" ]
    ++ lib.optionals withDocs [ "docs" ]
    ++ lib.optionals withCoverage [ "coverage" ]
  );
  version = "git";

  src = sources;

  nativeBuildInputs = [
    cmake
    ninja
  ];

  buildInputs = [
    boost
  ]
  ++ lib.optionals withTests [ catch2 ]
  ++ lib.optionals withDocs [
    docs.doxygen
    docs.python
  ];

  dontUseCmakeBuildDir = true;

  cmakeBuildType = if withDebug then "Debug" else "Release";

  cmakeFlags = [
    (lib.cmakeBool "zug_BUILD_TESTS" withTests)
    (lib.cmakeBool "zug_BUILD_EXAMPLES" withExamples)
    (lib.cmakeBool "zug_BUILD_DOCS" withDocs)
    (lib.cmakeBool "ENABLE_COVERAGE" withCoverage)
  ];

  # Build test targets before running them
  ninjaFlags = lib.optionals withTests [ "tests" ] ++ lib.optionals withExamples [ "examples" ];

  doCheck = withTests;
  checkTarget = "check";

  meta = {
    homepage = "https://github.com/arximboldi/zug";
    description = "Transducers for C++";
    license = lib.licenses.boost;
  };
}
