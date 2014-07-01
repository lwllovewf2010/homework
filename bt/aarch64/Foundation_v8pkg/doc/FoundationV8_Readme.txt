FVP v8 Foundation model 0.8 build 5206
==============================================

Copyright (C) 2013 ARM Limited, All rights reserved

Contents:

* Model overview
* Documentation and support
* Known issues and limitations

Model overview
--------------

This v8 Foundation model provides a basic ARMv8 platform environment suitable
for running bare metal semi-hosted applications, and Linux OS booting. The
platform provides:

- An ARMv8 processor cluster with 1-4 CPUs
- 8GB of RAM
- 4 PL011 UARTs exposed as host TCP sockets
- Platform periperals including a Real-time clock, Watchdog timer,
  Read-time timer and Power controller
- Secure peripherals including a Trusted watchog, Random number generator,
  Non-volatile counters and Root-key storage
- A network device model connected to host network resources
- A block storage device implemented as a file on the host
- A small system register block

The V8 processor model implements:

- AArch64 at all exception levels
- AArch32 support at EL0/EL1
- Little and big endian at all exception levels
- Generic timers
- Self-hosted debug
- GICv2 and optional GICv3 memory mapped CPU interfaces and distributor
- No support for Thumb2EE
- No support for Crypto extensions

The models runs as fast as possible unless all the CPUs in the cluster are
in WFI/WFE, in which case the model idles until an interrupt or external
event occurs.

Caches are modelled as stateless and there is no write buffer. This gives
the effect of perfect memory coherence on the data side. The instruction
side has a variable size prefetch buffer so requires correct barriers to
be used in target code to operate correctly.

There are two styles of network support:

NAT, IPv4 based networking provides limited IP connectivity by using user
level IP services. This requires no extra privileges to set up or use but
has inherent limitations. System level services, or services conflicting
with those on the host can be provided using port remapping.

Bridged networking requires the setup of an Ethernet bridge device. This
usually requires administrator privileges. See the documentation in the
Linux bridge-utils package for more details.

Documentation and support
-------------------------

All model configuration is provided by command line arguments. Run the model
with the --help command line option to get a summary of the available
commands.

Further model documentation including the modelled peripherals, system memory
maps, interrupt routing and semi-hosting support can be found in DUI0677, the
Foundation Model User Guide.

ARM does not offer direct customer support for the Foundation Model. For
technical support use the ARM support forums, http://forums.arm.com.

Enhancements and changes in the 0.8 build 5206 release
---------------------------------------------------------
- The Foundation model has been revised to support the ARM Trusted Base
  System Architecture (TBSA) and Server Base System Archicture (SBSA).
  A number of peripherals have been added with corresponding changes in
  the memory map. These changes are documented in DUI0677 the Foundation
  Model User Guide, issue C and later.

- An optional GICv3 controlled by the --(no-)gicv3 command line options
  has been added to the Foundation v2 model.

- The system register block has been updated to add an ID register. This
  allows target code to distinguish between Foundation v1 and Foundation v2
  models; and between models using the GICv2 legacy memory map, and GICv3 map.

- Initialisation and reset issues with the virtio block device affecting
  Linux KVM have been fixed.
  (TA-566785/SDDKW-23448/SDDKW-24825)

Enhancements and changes in the 0.8 build 4850 release
---------------------------------------------------------

- To bring the Foundation model in line with other Fast Models packages, the
  directory structure of the install has been changed. See section 2.1 of
  DUI0677, the Foundation Fast Model User Guide for details.

- Stability and performance improvements to the virtio block device when
  used for I/O intensive workloads.
  (TA-536363/TA-543995/SDDKW-20727)

- Allow virtio block devices to use read-only images and add a --read-only
  option to force writeable images to be read-only.
  (SDDKW-21945)

- Add the --nsdata command line option to allow data to be loaded into
  non-secure memory when the --secure-memory is enabled.
  (SDDKW-21078)

- Add a --uart-start-port option to configure the TCP port range used by
  the Foundation model UART to listen for connections. This allows
  many Foundation models to be started on the same host.
  (SDDKW-21677)

- Allow the system global counter module to count using real time instead
  of simulated time when the --use-real-time option is provided.
  (SDDKW-20933)

Known issues and limitations
----------------------------

This version of the model has the following issues and limitations:

Networking support currently uses an SMSC Ethernet model rather than a
virtio-based device model.

NAT (aka user mode) network support has known issues which cause networking
to intermittently slow down.

The processor reports an MIDR with 0x41 as the implementer code, 0xF as the
architecture code, 0xD00 as the part number, and the major and minor revision
of the model. For example, for Foundation v8 r2p5 this gives 0x412FD005.

There is no system level master timer block.

UARTs cannot be configured as enabled at reset. Bootloaders will need to do
this manually.

FVP v8 Foundation model 0.8 build 5206
