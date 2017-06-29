Iterative Spanning Forest - Superpixel Segmentation Demo

(c) 2016 John E. Vargas-Muñoz, Ananda S. Chowdhury, Eduardo B. Alexandre, Paulo A. Vechiatto Miranda, and Alexandre X. Falcão
------------------------------------------------------------------------------------------------------------------------------

This software contains superpixel segmentation programs corresponding to the paper 
"An Iterative Spanning Forest Framework for Superpixel Segmentation" submitted to IEEE TIP. 
The software consists of the IFT library (compiled for linux 64-bit) and five programs corresponding to the 
five ISF methods proposed in the paper above.

- Dependencies 
  - The IFT library has some dependencies that can be easily installed using apt-get in Ubuntu. 
  They are: liblapack-dev libblas-dev libatlas-base-dev libpng-dev zlib1g-dev.
  
- Instructions
  - The programs are available in the "demo" directory:
    iftISF_GRID_ROOT.c
    iftISF_MIX_ROOT.c
    iftISF_GRID_MEAN.c
    iftISF_MIX_MEAN.c
    iftISF_REGMIN.c
  
  - To compile the programs enter to the "demo" directory and execute the next command:
    make program_name_without_extension
    This command generates binary files in the directory "bin" with the same name as the program file.
  
  - Run the programs: 
    - To see the list of parameters of every program execute it without parameters.
    - The main parameters of the ISF methods (except ISF_REGMIN) are: the number of superpixels, 
      alpha and beta. As explained in the paper beta=12 works well for colored images and using alpha=0.12 
      we obtain a good trade-off between regularity and boundary adherence. The programs also allow 
      to perform smoothing to the final superpixel image, to do that you have to set the parameter 
      "smooth niters" to a value greater than zero. For example, to generate 150 superpixels without 
      smoothing you can run one of the next commands to obtain the results for ISF_GRID_ROOT, 
      ISF_MIX_ROOT, ISF_GRID_MEAN and ISF_MIX_MEAN, respectively:
      ../bin/iftISF_GRID_ROOT image1.png 150 0.12 12 10 0 output
      ../bin/iftISF_MIX_ROOT image1.png 150 0.12 12 10 0 output
      ../bin/iftISF_GRID_MEAN image1.png 150 0.12 12 10 0 output
      ../bin/iftISF_MIX_MEAN image1.png 150 0.12 12 10 0 output
      
    - The parameters for ISF_REGMIN are: the number of superpixels and area for the "area closing operation" 
    applied over the gradient image to delete small components in the final result 
    (if you do not want to use this operation just set area=0). For example, to generate 150 superpixels
    with area closing 50 run the next command:
      ../bin/iftISF_REGMIN image1.png 150 50 output

    - The commands above output the superpixel image in the PGM format "output.pgm" and 
      the superpixel borders superimposed over the original image "output.png".
