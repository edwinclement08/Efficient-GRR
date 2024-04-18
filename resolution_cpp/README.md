# Steps

1. Create Python Virtualenv using at least Python3.10

```bash
python3 -m venv venv
```

2. Install the requirements.txt using pip within the env

```bash
source ./venv/bin/activate
pip install -r requirements.txt
```

3. Build and test the CPP library using the following invoke commands

```bash
invoke build-pybind11
invoke test-pybind11
```