#E xample service usage:


Set output to ON:

rosservice call /hal_io_card/Out "pin: 12
state: 1" 

Read input:

rosservice call /hal_io_card/In "pin: 0"
