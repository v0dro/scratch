from distutils.core import setup, Extension 
  
module1 = Extension('scatter_add',
                    sources = ['scatter_add.c'])

setup (name = 'scatter_add',
       version = '1.0',
       description = 'This is a demo package',
       ext_modules = [module1])
