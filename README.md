# BubbleScope
Powering a bubble machine with Periscope's hearts #PoweredByHearts.

* [Reqs](#reqs)
* [Config](#config)
* [Install](#install)
    * [Node](#node)
* [Hardware](#hardware)
    * [Particle](#particle)
    * [Bubble Machine](#bubble-machine)
* [Bubble machine hack](#bubble-machine-hack)
    * [Adding a power control](#adding-a-power-control)
    * [Speeding up the bubble wheel](#speeding-up-the-bubble-wheel)
    * [Powering the bubble machine with 6V](#powering-the-bubble-machine-with-6V)

## Reqs
* `Particle` (`Arduino` like dev kit)
* `node`

## Config
Edit `config.json` file at the root of the project directory to set your Particle parameters, see http://docs.particle.io/photon/api/.

```json
{
    "device_name": "",
    "access_token": ""
}
```

## Install
```shell
git clone git@github.com:vjo/bubblescope.git bubblescope && cd bubblescope
```

### Node
```shell
cd server/node && npm install
```
`bubblescope` uses the [peristream](https://github.com/ArnaudRinquin/peristream) project that helps you listening to a Periscope stream.

### Particle
Using [particle-cli](https://github.com/spark/particle-cli) tool:
```shell
cd client
particle compile photon particle # replace "photon" with your target device type
particle flash DEVICE_NAME PATH_TO_BIN
```

## Hardware

### Particle
In this project we are using a [Spark Core](https://www.particle.io/prototype#spark-core "Spark Core"), from [Particle](https://www.particle.io "Particle.io") (formerly Spark) to receive hearts events and control the bubble machine. A `Spark Core` is an Arduino like hardware development kit for the internet of things, you may be able to re-use our code on other Arduino platforms but you will need to build your own REST API to communicate with your board as it comes for free with a `Particle` board.

### Bubble Machine
We are currently using a QTX Sound mini party bubble machine, mainly because it was cheap and offered the possibility to run on battery.

However, every bubble machine should do the trick.

## Bubble machine hack

### Adding a power control
To control the bubble machine via the spark core, we decided to open the control circuit by adding to wire after the main control switch already on the machine. This allow us to still use the main switch to power the machine and then to use a relay to command the machine with the spark core.

Tools needed:
* 2 electric wire
* Soldering iron

The first step consist to unsolder the right part of the main switch and solder one of our wire on the switch pin. Then solder our other wire to the wire that was originally connected to the main switch.

Then you can use the ventilation grill to pass the wire through and properly reassemble the bubble machine.

Here are the result two pictures, as it might help to understand the hack:

![Power control hack](https://github.com/vjo/bubblescope/blob/master/misc/power-control-hack.jpg "Power control hack")

![Power control hack zoom out](https://github.com/vjo/bubblescope/blob/master/misc/power-control-hack-zoom-out.jpg "Power control hack zoom out")

### Speeding up the bubble wheel
We were a bit disappointed by the speed of the bubble wheel not allowing us to create as many bubble as we were hopping.

However, by disassembling the machine we have been able to short-circuit the gear transmission and speed up the wheel.

Tools needed:
* Screwdriver
* 2 small screws or something you can use to raise the height of a gear

With the help of a screwdriver open the bubble machine to free the rotor mechanism. Open it to remove the bubble wheel, it should look like this:

![Transmission gear hack 0](https://github.com/vjo/bubblescope/blob/master/misc/transmission-gear-hack-0.jpg "Transmission gear hack 0")

Then, remove all the white plastic gears:

![Transmission gear hack 1](https://github.com/vjo/bubblescope/blob/master/misc/transmission-gear-hack-1.jpg "Transmission gear hack 1")

Now add 2 small screws to raise the height of the first gear:

![Transmission gear hack 2](https://github.com/vjo/bubblescope/blob/master/misc/transmission-gear-hack-2.jpg "Transmission gear hack 2")

Place the first gear back and you are good to reassemble all the parts:

![Transmission gear hack 3](https://github.com/vjo/bubblescope/blob/master/misc/transmission-gear-hack-3.jpg "Transmission gear hack 3")

By raising up the first gear the rotation power go directly from the first to the last gears and allow the bubble wheel to turn faster. We now produce more bubble for more fun.

### Powering the bubble machine with 6V
The QTX Sound mini party bubble machine is made to run with a 3V power supply. Anyway, we wanted a way to make the fan turn faster to blow and push the bubbles further.

With the help of a battery rack and 4 AA batteries, we are simply powering the bubble machine with 6V and it works well.
