const express = require("express");
const router = express.Router();

router.post("/values", async (req, res) => {
  try {
    const { temp_val, humidity_val, distance_val, light_val, ph_value } = req.body;
    const values = {
      temp: temp_val,
      humidity: humidity_val,
      distance: distance_val,
      light: light_val,
      ph_value: ph_value
    };
    console.log(req.body);

    global.io.sockets.emit("values_updates", { values: values });

    return res.status(200).send({
      message: "Values Receivbed",
      values: values,
    });
  } catch (error) {
    console.log(error);
    return res.status(500).send({ message: error });
  }
});

module.exports = router;
