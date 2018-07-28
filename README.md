# SimTV

**An Arduino based TV simulator.**

A TV simulator is a device that is supposed to keep burglars from breaking into your home, by simulating the distinctive color patterns that resemble those of a real TV. From the outside, it gives the impression that someone is at home, watching TV.

This TV simulator uses an Arduino Uno and a RGB LED strip.

A feature is that, instead of random color patterns, an actual movie is used as source. Because, what would look more realistic than a real movie?

Besides the Arduino sketch, this project also contains a tool for converting your own favorite movie.

Read more about this project [in my blog article](https://shred.zone/cilla/page/379/an-arduino-tv-simulator.html).

## Hardware

You need:

* Arduino Uno
* RGB shield, e.g. [Velleman KA01](http://www.velleman.co.uk/contents/en-uk/p578_ka01.html). Other RGB shields may also work, with some minor modifications to the Arduino sketch.
* RGB LED strip, 12 V, about 7 W. The KA01 needs an LED strip with Common Anode.
* 12 V power supply that can power the Arduino and the LED strip

If your RGB shield is a constructional kit, you need to assemble it by instructions. After that, connect the shield to the Arduino, and the RGB LED strip to the shield. That's all.

**Caution:** The RGB strip is a very intense light source that can cause permanent damage to your eyes! Avoid looking directly into the LEDs!

## Software

[simtv/simtv.ino](./simtv/simtv.ino) is the Arduino sketch of the TV simulator. An example movie stream can be found in [simtv/stream.h](./simtv/stream.h), which is included into the sketch. Just build the sketch and install it on your Arduino. The movie runs about 2 hours, and restarts after that.

To use your own movie streams, you need to encode the movie to a `stream.h` file using the `simencode.py` tool.

## Encoding

The `simencode.py` tool reads a movie file and converts it to a `stream.h` file that can be included in the Arduino sketch. The conversion is very effective, so even a feature film can be used.

Just invoke it by passing the file name of the movie:

```sh
./simencode.py path/to/movie.mp4
```

It will convert the movie file and overwrite the `sketch/stream.h` file with the result. Depending on the OpenCV installation, the conversion time taken might be equal to the runtime of the movie.

There are some options that might improve the result of the simulated movie:

* `-o`, `--out`: By default, `simencode.py` writes to `sketch/stream.h`. You can change the file name here.
* `-x`, `--frames`: Encoding will stop after processing this number of frames. Useful for testing.
* `-r`, `--fps`: Playback frames per seconds. Defaults to 10, but can be lowered if the converted stream does not fit into Arduino's memory. 60 should be divisible by this value.
* `-g`, `--gamma`: Change the gamma correction. Defaults to 2.2, which is a standard decoding gamma for displays.
* `-b`, `--brightness`: Change the brightness by this factor. Defaults to 1.0, which leaves the brightness unchanged. If you convert a letterbox movie, some parts of the screen are permanently black, darkening the overall result. With this option, you can bring the brightness back to normal levels. If the value is too high, color clipping may occur.
* `-S`, `--soft`: If the hue or brightness difference of two colors exceed this threshold, it is regarded as a soft color change. SimTV will gradientally change from the previous color to the new color. Defaults to 7%. Lower values give better results, but also larger stream files that may not fit into Arduino's memory.
* `-H`, `--hard`: If the hue or brightness difference of two colors exceed this threshold, it is regarded as a hard color change. SimTV will immediately change to the new color. Defaults to 20%. Lower values give better results, but also larger stream files that may not fit into Arduino's memory.

### Requirements

The encoder is written in Python 3. It requires [OpenCV](https://opencv.org/) and the corresponding Python library.

On Fedora, you can install the requirements with:

```sh
sudo dnf install opencv opencv-python3 gstreamer1-libav
```

## Contribute

* Fork the [Source code at GitHub](https://github.com/shred/simtv). Feel free to send pull requests.
* Found a bug? [File a bug report!](https://github.com/shred/simtv/issues)


## License

_simtv_ is open source software. The source code is distributed under the terms of [GNU General Public License (GPLv3)](https://www.gnu.org/licenses/gpl-3.0.en.html#content).
