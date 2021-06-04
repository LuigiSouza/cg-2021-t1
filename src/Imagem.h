
#ifndef ___IMAGEM__H___
#define ___IMAGEM__H___

void TransformadaUnidimensional(float size)
{
    float OFFSET = 40.0;
    float LEN = 400.0;
    float AMPLITUDE = 20.0;

    float m = size;
    for(float u = 0; u < size; u++){
        int div = (int)u/4;
        int mod = (int)u%4;
        CV::translate(OFFSET + div*(OFFSET + LEN), OFFSET + (mod*80));

        CV::text(-10, 15, "1");
        CV::text(-10, -5, "0");
        CV::text(-20, -25, "-1");
        CV::line(0, 0, LEN, 0);

        for(float j = 0, x = 0; x < LEN; j += 0.005, x += 0.1)
            CV::point(x, cos((2 * j + 1)*u*PI/(2 * m))*AMPLITUDE);

    }

    CV::translate(0, 0);
}

void CarregarImagem(Bmp * bmp) {

    CV::translate(50, 50);

    uchar * data = bmp->getImage();
    int bytesLine = bmp->getCanal();

    for(int x = 0; x < bmp->getWidth(); x++)
    for(int y = 0; y < bmp->getHeight(); y++) {
        CV::color(float(data[bytesLine*(y*bmp->getWidth()+x)+0])/255,
                  float(data[bytesLine*(y*bmp->getWidth()+x)+1])/255,
                  float(data[bytesLine*(y*bmp->getWidth()+x)+2])/255);

        CV::point(x, y);
    }

    CV::translate(0, 0);
}

#endif
