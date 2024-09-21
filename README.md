# Renderer

A simple implementation of the computer graphics rendering pipeline from a set of vertices to the rasterized image.
Written for educational purposes and not for performance.

## Building and Running

Build the code via `make`

```bash
make
```

Then call the binary either alone, or with a single parameter to define a specific test scene.

```bash
./renderer [<scene-identifier>]
```

Available scene-identifiers:
- `triangle`
- `axis`
- `cube`
- `d20`
- `kokiri`
- `psyduck`
- `teapot`
