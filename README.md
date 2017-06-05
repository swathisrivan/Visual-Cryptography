# Visual-Cryptography

Author : Swathi Sridhar(swathisr@usc.edu)

--Implementation of a secretive image sharing scheme similar to Doug Stinson's Visual Cryptography

--Every resultant encrypted pixel of the image was obtained by superimposing to different intermediate image shares based on a random key bit value generated using an MD5 stream cipher

--Depending on the nature of the input pixel(either black or white) the corresponding shares based on the key pixel is generated and are superimposed

--The resultant to the original image size ratio would be 4:1 and the decryption was done based on the combination of the four pixels in the encrypted image for every pixel
