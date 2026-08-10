# The Iris Plaques (2026)

Iris melanoma eye plaque geometries accompanying
[Djedouani et al., Med Phys. 2026;e70439](https://doi.org/10.1002/mp.70439).

## Models

| Directory | Description | Seeds |
| --- | --- | ---: |
| `Iris180/` | 180° iris plaque | 6 |
| `Iris270/` | 270° iris plaque | 10 |
| `Iris270_No_Lips/` | 270° iris plaque without collimating lips | 10 |
| `Iris360/` | 360° iris plaque | 14 |
| `COMS_No_Insert/` | COMS-style plaque without silastic insert (comparison model) | 10 |

Each model directory generally includes:

- `*.geom` — heterogeneous plaque materials (Modulay / Silastic as applicable)
- `*_water.geom` — all-water counterpart
- `source_transformation_*.geom` — seed placement transforms
- `*_egsview.png` — preview image (where available)

`Iris270_No_Lips/` does not yet include an `*_egsview.png` preview.

`COMS_No_Insert/` reuses the Iris270 seed layout via
`source_transformation_COMS_No_Insert.geom`.

Example egs_view image of the 270° iris plaque:

[Iris plaque with 270 degree coverage](./Iris270/Iris270_egsview.png)

## Attribution

Geometries and associated inputs: Marwa Djedouani.
Uploaded to this repository by Matt Inglis-Whalen.
