return {
  version = "1.1",
  luaversion = "5.1",
  orientation = "orthogonal",
  width = 16,
  height = 12,
  tilewidth = 80,
  tileheight = 80,
  properties = {},
  tilesets = {
    {
      name = "desert",
      firstgid = 1,
      filename = "desert.tsx",
      tilewidth = 80,
      tileheight = 80,
      spacing = 0,
      margin = 0,
      image = "res/tiles.png",
      imagewidth = 320,
      imageheight = 240,
      properties = {},
      tiles = {}
    }
  },
  layers = {
    {
      type = "tilelayer",
      name = "Tile Layer 1",
      x = 0,
      y = 0,
      width = 16,
      height = 12,
      visible = true,
      opacity = 1,
      properties = {},
      encoding = "lua",
      data = {
        1, 5, 9, 1, 5, 9, 1, 5, 9, 1, 5, 9, 6, 7, 8, 1,
        5, 1, 5, 5, 1, 5, 5, 1, 5, 5, 1, 5, 10, 11, 12, 5,
        9, 5, 1, 9, 5, 1, 9, 5, 1, 9, 5, 1, 9, 5, 1, 9,
        1, 9, 5, 2, 3, 4, 1, 9, 5, 1, 9, 5, 1, 9, 5, 1,
        5, 1, 9, 6, 7, 8, 5, 1, 9, 5, 1, 9, 5, 1, 9, 5,
        9, 5, 1, 10, 11, 12, 9, 5, 1, 9, 5, 1, 9, 5, 1, 9,
        1, 9, 5, 1, 5, 9, 1, 5, 9, 1, 5, 9, 1, 5, 9, 1,
        5, 1, 9, 5, 1, 5, 5, 1, 5, 5, 1, 5, 5, 1, 5, 5,
        9, 5, 1, 9, 5, 1, 9, 5, 1, 9, 5, 1, 9, 5, 1, 9,
        1, 9, 5, 1, 9, 5, 1, 9, 5, 1, 9, 5, 1, 9, 5, 1,
        5, 1, 9, 5, 1, 9, 5, 2, 3, 4, 1, 9, 5, 1, 9, 5,
        9, 5, 1, 9, 5, 1, 9, 6, 7, 8, 5, 1, 9, 5, 1, 9
      }
    }
  }
}
