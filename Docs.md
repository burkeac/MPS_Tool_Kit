# MPS Toolkit Documentation

## Welcome to the MPS Toolkit documentation.
This kis is currently in very early development and therefor documentation.

The following file structure is used throughout this library.
```
|- main
|
|- libSrc
|    |
|    |- Color
|    |    |- Files for color module...
|    |
|    |- TransferFunctions
|         |- Files for transferFunction module...
|
|- appSrc
|    |- Files for CLI apps...
```
## Module: ColorSpace
### Enumerations:
```
enum    MPS::ColorSpace{
         Rec709,
         Rec2020,
         P3D65,
         P3DCI,          
         ACES_P0, 
         ACES_P1
        }
```
### Classes
```
colorPrimaries {
        public:
            colorPrimaries();
            colorPrimaries(int colorSpace);
            std::vector<float> Primaries;
            std::vector<float> WhitePoint;
            void selectPrimary(int colorspace);
            void printPrimaries();
            
    };
```
