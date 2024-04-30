# RIPencapsulation USENIX WOOT 24 Artifact Evaluation

RIPencapsulation is an interrupt-based side channel attack which exploits the vulnerabilities present 
in the Trusted Execution Environment provided on Texas Instruments MSP microcontrollers, IP Encapsulation
(IPE), namely -- a) lack of call site verification on IPE entries and b) no context-clearing on IPE exits --
to exfiltrate or modify the protected IPE code and data.

The attack consists of three phases —- 1) CPU register state dumps, 2) reverse-engineering, and 
3)firmware exfiltration -— explained in more detail in "RIPencapsulation: Defeating IP Encapsulation on
TI MSP devices". The main goal of this artifact is to exfiltrate the IPE memory and demonstrate the 
end-to-end attack on multiple security-critical libraries compiled at different optimization levels.

The artifact also contains additional reverse-engineering scripts, old evaluation results as well 
proof-of- concept code for the MSP430P401R (discontinued), in the interest of facilitating future 
research on the topic.

## Software pre-requisites

* git client
* make
* msp430-gcc version >= 9.3.1
* mspdebug
* CCStudio

mspdebug has the following dependencies:
* libusb-dev
* libreadline-dev
* gcc
* make
* libmsp430.so

All of which except libmsp430.so can be installed on Ubuntu systems using `sudo apt install gcc make 
libusb-dev libreadline-dev`. libmsp430.so is the driver library mspdebug requires to interface with MSP430 devices
-- it is available as part of msp430-gcc, but needs to be made available to mspdebug using the following commands.
```
sudo cp ~/ti/msp430-gcc/bin/libmsp430.so /usr/local/lib/
echo "export LD_PRELOAD=/usr/local/lib/libmsp430.so" >> ~/.bashrc
source ~/.bashrc
```
mspdebug should not be installed from apt as the version there is outdated; use `git clone https://github.com/dlbeer/mspdebug`
to get the most recent version and follow the instructions in the README therein to install. Install msp430-gcc from 
https://www.ti.com/tool/MSP430-GCC-OPENSOURCE. Install CCStudio from https://www.ti.com/tool/CCSTUDIO.

## Hardware pre-requisites

To reproduce the end-to-end attack, you need the MSP430FR5994 launchpad. If interested, you can also verify the proof-of-concept
version of the attack on the MSP432P401R launchpad (discontinued).

## Experiment workflow

To verify the functionality of the end-to-end attack in exfiltrating the protected IPE firmware on the MSP430FR5994 launchpad, 
we recommend following this example workflow:

1. In `msp430/Makefile`, uncomment the `BENCHMARK` and `OPTFLAG` you wish to evaluate.
2. In `eval/python/RIPencapsulation.py`, uncomment the benchmark you wish to evaluate.
3. On the terminal, change the working directory to `eval/python/` by running the following command:
`cd ∼/path/to/your/project/directory/eval/python/`
4. Connect the MSP430FR5994 launchpad (via the USB debug cable) to your workstation and run the python script `RIPencapsulation.py`
by entering the following command on the terminal: `python3 RIPencapsulation.py`
5. Give the script some time to exfiltrate the IPE firmware. If the script fails in the middle (due to serial communication error),
press `CTRL+C` to break out of the script and then rerun it. In some cases, it might be required to hard reset the device (by unplugging
the launchpad and plugging it back in) before a rerun.
6. If the script finishes execution displaying "Unsuccessful: Could not find any indirect load instructions" this means that the number of
register dumps taken was insufficient for finding any indirect load instructions. This can be solved by changing the `reg_dump_size`
variable (line 24 of `eval/python/RIPencapsulation.py`) to a higher value. Its current value is set to work for `BENCHMARK = aes` and
`OPTFLAG = -O0`. Note that the higher its value, the more time it will take to exfiltrate the IPE memory.
7. Once the script finishes executing successfully, the exfiltrated dumps can be found in the `eval/dumps/` directory. The exfiltrated
IPE firmware can be found in the file `eval/dumps/exfiltrated_firmware.xlsx`, where the first column is the IPE address location and the
second column is the memory value at that location.
