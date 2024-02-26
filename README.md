
Retrieve BeagleV-Fire gateware design name and version from this utility running on BeagleV-Fire.

This utility can be built directly on BeagleV-Fire:

```
git clone https://github.com/vauban353/bvf-gateware-info.git
cd bvf-gateware-info
gcc -o gateware-info gateware-info.c
```

Once built executed it as root:
```
sudo ./gateware-info
```

This should give you something similar to this:
```
Gateware Design Information: 
  Design name: MY_CUSTOM_FPGA_DESIGN_94FD95F9
  Design version: 
       raw: 0x4296
       interpreted as unique-design-version: 17.04.6
       interpreted as date and time: 
         day: 11 
         time: 20:06
```


