## Onboard FL training, privacy and security

For TinyPART project, low-resource embedded devices such as Nordic nRF52840,Nano 33BLE Sense having chipset nRF5240 with 32-bit ARM Cortex M4 CPU withfloating point unit running at 64 MHz, 1MB of flash and 256 KB RAM.


To carry out FL workflows, [RIOT OS](https://www.riot-os.org/) is being used in C++. Our experiments include
- **ML training on clients:** generating synthetic dataset with low-dimensions features space and training linear regression models eg. 4 trained parameters. All the matrix computation is written from scratch to cope up with memory constraits on embedded device.
- Clients after conducting local training share trained parameters with server using CoAP layer in RIOT.
- Server reads these parameters from all clients and does aggregation (typically avearge).
- Clients read this aggregated parameter from CoAP from server and resume their local training on that.

### Extension of this work includes 
- **Countering attacks from Server using homomorphic encrption:** Our current FL flow has CoAP for exchanging model parameters between server and clients. So, this work should focus on encrypting these parameters (typically small vector of four elements) and sending to server which could then carry out basic arithmetic (add and multiply) on encrypted parameters. And finally, decrypting these parameters from server on client side. All these cryptography operations are done using third-party library (eg. https://github.com/bogdan-kulynych/libshe) in C++.
- **Distributed Differential Privacy:** where clients can add local noise to parameters before sharing with server. In case, where gradients are being shared instead of models parameters, so the gradients can also be clipped before sharing with server to avoid privacy leakage.