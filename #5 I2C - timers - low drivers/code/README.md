# Description

You will be asked to implement the I2C protocol, you aren’t allowed to use delays

1. Master Sends start signal
2. Master sends slave address 24
3. Slave confirms and send an ack
4. Master sends byte
5. Slave confirms with an ack
6. Repeat 4,5 till master sends 200 bytes
7. Stop the clock as a stop signal

Since you all are working from home you can implement both the master and the slave on a single arduino, but take care that this will require even more strict timing because the master can only do action while the slave can’t and vice versa (Use 2 pins for master and other 2 for
slave)
