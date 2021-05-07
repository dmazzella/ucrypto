# Compiling the cmodule into MicroPython

To build such a module, compile MicroPython with an extra make flag named ```USER_C_MODULES``` set to the directory containing all modules you want included (not to the module itself).

```python
import _crypto
```

## Examples
- [tests](https://github.com/dmazzella/ucrypto/tree/master/tests)
