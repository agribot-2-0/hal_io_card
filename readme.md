#Example service usage:

rosservice call /hal_io_card/control "{set_do_port0: true, port0_mask: 0x01, set_do_port1: true, port1_mask: 0x04, read_port0: true, read_port1: true}"