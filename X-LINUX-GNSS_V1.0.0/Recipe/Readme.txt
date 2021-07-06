
Adding the Recipe to your Image
Add Layer Path
After running source oe-init-build-env, the file location of your layer should be added to your bblayers.conf file. This file is located in build/conf/bblayers.conf. Enter the location of the layer in:
        BBLAYERS ?=
This is what the conf/bblayers.conf for meta-wolfssl on the Minnowboard looks like:
        BBLAYERS ?= " \
        /home/name/poky/meta \
        /home/name/poky/meta-yocto \
        /home/name/poky/meta-yocto-bsp \
        /home/name/poky/meta-intel \
        /home/name/poky/meta-minnow \
        /home/name/wolfSSL/meta-wolfssl \
        "
Append the Package
Within the build/conf/local.conf file, add this line anywhere:
        IMAGE_INSTALL_append = " cyassl benchmark ctaocrypttest"
We included all three recipe packages from meta-wolfssl here. You can add multiple recipes from different layers (as long as these layers have a path located in step one) here or just one, or none.
Add Package to Image Recipe
If you installed Poky from the Yocto Project, the recipe for your image will be located at meta/recipes-<name>/images/<name-of-recipe>.bb. You can create your own custom image recipe, or add to an existing one. Since we used core-image-sato, we edited core-image-sato.bb in this location. If you do this, make sure you know exactly which changes you made so you can revert back to the original image if needed. To add a new recipe to your image, add this line (or add to the IMAGE_INSTALL line if it already exists):
IMAGE_INSTALL += "package-name package2-name package3-name"
Run bitbake Command
After you have created and inserted everything into your image, you can bitbake the image and run it on your board. You can also bitbake each recipe individually to check for errors before trying to build the entire image. This can be done simply by typing:
bitbake <recipe-name>
where <recipe-name> is the directory where your .bb file is stored. Then you will run bitbake image-name
Types of Pre-Built Images
A list of the various Yocto Project images can be found here. The image we used for building with meta-wolfssl on the Minnowboard was core-image-sato. This image takes quite a while to build - expect over a day if you are building inside a virtual machine. It is also very large - not the image itself, but the build components take up almost 50GB, so make sure you have enough space on your computer. The actual image that will be booted onto your embedded device will be very small.
