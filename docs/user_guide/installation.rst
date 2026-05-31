Installation
============

This guide shows you how to build and install ``mcmpy``.
There are two supported installation methods for this project:

1. Installation using Docker
2. Local installation using CMake


Installation using Docker
-------------------------

First `install Docker <https://docs.docker.com/get-started/get-docker/>`_.
Create or navigate to the folder that contains all input files necessary for the data analysis and save the following block of code as a file named ``docker-compose.yml``:

.. code-block:: yaml

   services:
      mincompspin:
        image: ghcr.io/dm-lab-uva/mincompspin:latest
        ports:
          - "8888:8888"
        volumes:
          - .:/MinCompSpin
        command: jupyter notebook --ip=0.0.0.0 --no-browser --allow-root

To launch the container environment, open a terminal in your directory and execute the following command:

.. code-block:: bash

   docker compose up

When running the command for the first time, it will download the pre-built Docker image and automatically start a jupyter server.
This returns a link beginning with ``127.0.0.1``, which can be opened in a local browser.
Next times, it will not download the latest Docker image but just launch a container with the latest downloaded version.
If a newer version of the image exists, run the following command first:

.. code-block:: bash

    docker compose pull

Local installation using CMake
------------------------------

This method requires some system dependencies. Make sure the following are installed:

- C++ compiler with C++ 11 support
- CMake >= 3.26.4
- Python >= 3.11
- NumPy

Clone the GitHub repository and run the installation script:

.. code-block:: bash

   git clone --recursive git@github.com:DM-Lab-UvA/MinCompSpin.git
   cd MinCompSpin
   bash installation.sh

.. note::

   By default, the tests are built but not run.
   To build the project without the tests, execute the following commands instead: 

.. code-block:: bash

   git clone --recursive git@github.com:DM-Lab-UvA/MinCompSpin.git
   cd MinCompSpin
   mkdir -p build && cd build
   cmake .. -DBUILD_TESTS=OFF
   make
   cd mcmpy
   pip install .

Testing
-------

After installation, the test suite can be run to verify if the installation was succesful:

.. code-block:: bash

   cd build
   ctest --output-on-failure

Using MCMPy
===========

Once `mcmpy` is installed, it can be imported in Python.

.. code-block:: python

   import mcmpy
   import numpy as np
