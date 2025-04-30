# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

import os
import sys
sys.path.insert(0, os.path.abspath('../build/mcmpy'))

project = 'Minimally Complex Spin Models'
copyright = '2025, Aaron De Clercq'
author = 'Aaron De Clercq'
release = '0.1.0'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = []

templates_path = ['_templates']
exclude_patterns = []

extensions = [
    'sphinx.ext.autodoc',    # This enables the automodule directive
    'sphinx.ext.napoleon',   # Optional: For Google-style or NumPy-style docstrings
    'sphinx.ext.viewcode',   # Optional: Adds "view source" links in HTML
]



# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'sphinx_rtd_theme' # or 'furo', 'sphinx_rtd_theme', etc. alabaster
html_static_path = ['_static']
