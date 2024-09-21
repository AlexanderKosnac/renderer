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
- `axis`
- `cube`
- `d20`
- `kokiri`
- `psyduck`
- `teapot`
- `triangle`

You can compile and run some tests with the `test` target:

```bash
make test
```


## State of the Implementation

As of now, basic `.obj` files can be read and rendered.
This means only comments (`#`), vertices (`v`), and faces (`f`) are properly processed.
See the files included in this repository as examples.
There is no support for textures.
Texture coordinates and faces referencing them are not properly read, **do not** include them!