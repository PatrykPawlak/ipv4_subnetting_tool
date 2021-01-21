# IPv4 subnetting tool

## About

IPv4 subnetting tool is a comand-line piece of software allowing you to load text file containing list of IPv4 addresses and generate text file with right indents for 
nested subnets. Also correct bad network IPv4 addresses (via calculations based on network/subnetwork mask).

## Usage
```
ipv4_subnetting_tool.exe -i InputFile.txt -o OutputFile.txt

Parameter List:
-h, --help              Displays help message and exit.
-i, --input             Define input file.
-o, --output            Define output file.
```

### Example input file structure 

**CIDR Comment (CIDR = IPv4/ShortSubnetMask)**
```
157.158.1.0/24 Subnet for client X
157.158.128.0/24 FREE
212.106.160.0/20 Allocation 4
212.106.160.0/21 Remote department
157.158.129.0/24 Department 1
157.158.130.0/24 FREE
157.158.128.0/17 Subnet for client Y
0.0.0.0/0 Full
157.158.2.0/24 Subnet for client Z
157.158.131.1/24 Department 2
157.158.0.0/16 Allocation 1
```

### Example output file structure
```
0.0.0.0/0 (0.0.0.0) Full
    157.158.0.0/16 (255.255.0.0) Allocation 1
        157.158.1.0/24 (255.255.255.0) Subnet for client X
        157.158.2.0/24 (255.255.255.0) Subnet for client Z
        157.158.128.0/17 (255.255.128.0) Subnet for client Y
            157.158.128.0/24 (255.255.255.0) FREE
            157.158.129.0/24 (255.255.255.0) Department 1
            157.158.130.0/24 (255.255.255.0) FREE
            157.158.131.0/24 (255.255.255.0) Department 2
    212.106.160.0/20 (255.255.240.0) Allocation 4
        212.106.160.0/21 (255.255.248.0) Remote department
```

## Limitations/Errors
- Comment **MUST** begin with letter

- Errors/log will be printed on command-line window. For example:

`ERROR: Cannot add subnet allocation from line [line_number] {{IPv4 error log} {Mask error log} {Comment error log}}`

`Attempt to open the file: [InputFile.txt] succeeded.`

`Attempt to write sorted subnets from structure to file. Please wait...`
