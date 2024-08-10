{
  description = "A development environment for my project";

  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";

  outputs = { self, nixpkgs }:
  let
    system = "x86_64-linux";
  in
  {
    devShells.${system} = import nixpkgs {
      system = system;
      config = { allowUnfree = true; };
    }.mkShell {
      buildInputs = with nixpkgs; [
        python39
        python39Packages.virtualenv
        git
        # Add other tools you need here
      ];

      shellHook = ''
        echo "Welcome to the development shell!"
      '';
    };
  };
}

