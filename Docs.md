# MPS Toolkit Documentation

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