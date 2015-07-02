# bubblescope - Particle
This is the code embedded in the particle core device

## Usage: 
Using spark-cli, compile and flash to your device 
```
spark flash <spark_device_name>
```

The core will control the output pin `D0`:
* `HIGH` when no heart is being received
* `LOW` for `MS_OF_BUBBLE` milliseconds (default 1000) while hearts has been received.
