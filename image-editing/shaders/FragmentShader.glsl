#version 410

// Input
layout(location = 0) in vec2 texture_coord;

// Uniform properties
uniform sampler2D textureImage;
uniform ivec2 screenSize;
uniform int flipVertical;
uniform int outputMode = 0; // 0: original, 1: grayscale, 2: blur

// Output
layout(location = 0) out vec4 out_color;

// Local variables
vec2 textureCoord = vec2(texture_coord.x, (flipVertical != 0) ? 1 - texture_coord.y : texture_coord.y); // Flip texture


vec4 grayscale()
{
    vec4 color = texture(textureImage, textureCoord);
    float gray = 0.21 * color.r + 0.71 * color.g + 0.07 * color.b; 
    return vec4(gray, gray, gray,  0);
}


vec4 blur(int blurRadius)
{
    vec2 texelSize = 1.0f / screenSize;
    vec4 sum = vec4(0);
    for(int i = -blurRadius; i <= blurRadius; i++)
    {
        for(int j = -blurRadius; j <= blurRadius; j++)
        {
            sum += texture(textureImage, textureCoord + vec2(i, j) * texelSize);
        }
    }
    float samples = pow((2 * blurRadius + 1), 2);
    return sum / samples;
}

void bubbleSort(inout float pixels[9])
{
    bool swapped = true;
    int j = 0;
    float tmp;
    for (int c = 0; c < 3; c--)
    {
        if (!swapped)
            break;
        swapped = false;
        j++;
        for (int i = 0; i < 3; i++)
        {
            if (i >= 3 - j)
                break;
            if (pixels[i] > pixels[i + 1])
            {
                tmp = pixels[i];
                pixels[i] = pixels[i + 1];
                pixels[i + 1] = tmp;
                swapped = true;
            }
        }
    }
}



vec4 median()
{
    float pixels[9];
    vec4 rgb_pixels[9];
    mat3 dx = mat3(
    -1, 0, 1,
    -1, 0 ,1, 
    -1, 0, 1);
    mat3 dy = mat3(
    1, 1, 1, 
    0, 0, 0,
    -1, -1, -1);
    vec2 texelSize = 1.0f / screenSize;
    for(int i = 0; i<3; i++)
    {
        for(int j = 0; j<3; j++)
        {
            vec4 color = texture(textureImage, textureCoord + vec2(dx[i][j], dy[i][j]) * texelSize);
            pixels[i*3 + j] = 0.21 * color.r + 0.71 * color.g + 0.07 * color.b;
            rgb_pixels[i*3 + j] = color;

        }
    }
   for (int i =0; i<9;i++)
    {
        for(int j = i  + 1; j<9; j++)
        {
            if (pixels[j] < pixels[i])
            {
                float aux = pixels[j];
                pixels[j] = pixels[i];
                pixels[i] = aux;
            }
        }
    }
   for(int i = 0; i < 9; i++)
   {
     if (0.21 * rgb_pixels[i].r + 0.71 * rgb_pixels[i].g + 0.07 * rgb_pixels[i].b == pixels[4])
        return rgb_pixels[i];
   }
}

vec4 sobel()
{
    vec2 texelSize = 1.0f / screenSize;
	float gx[9] = float[](-1, 0, 1, -2, 0, 2, -1, 0, 1);
	float gy[9] = float[](1, 2, 1, 0, 0, 0, -1, -2, -1);
    float dx = 0;
    float dy = 0;

    float dc = 0;
    for(int i = -1; i <= 1; i++) {
		for(int j = -1; j <= 1; j++) {
            vec4 curr = texture(textureImage, textureCoord + vec2(i, j) * texelSize);
            float gray = 0.21 * curr.r + 0.71 * curr.g + 0.07 * curr.b;
			dx += curr.x * gx[3 * (i + 1) + (j + 1)];
		}
	}

	for(int i = -1; i <= 1; i++) {
		for(int j = -1; j <= 1; j++) {
            vec4 curr = texture(textureImage, textureCoord + vec2(i, j) * texelSize);
            float gray = 0.21 * curr.r + 0.71 * curr.g + 0.07 * curr.b;
			dy += curr.x * gy[3 * (i + 1) + (j + 1)];
		}
	}

    dc = sqrt(dx*dx + dy*dy);
    double theta = atan(dy, dx);
    //return atan(dy/dx);
    //float theta = atan(dy, dx); 
    //if (theta >= 0  && )
    int dir = 0;
    double M_PI = 3.14159265358;
    theta = 180/M_PI * theta;
    if (theta < 0)
        theta +=180;
    if ((0 <= theta && theta < 22.5) || (157.5 <= theta && theta <= 180)) {
        dir = 1;
    } else if(22.5 <= theta && theta < 67.5) {
        dir = 2;  
        //return vec4(0,1,0,0); 
    } else if(67.5 <= theta && theta < 112.5) {
        dir = 3;
        //return vec4(0,0,1,0);
    } else if(112.5 <= theta && theta < 157.5) {
        dir = 4;
        //return vec4(0,0,0,0);
    }


    if (dir == 1) {
    
        float da = 0, db = 0;
        for(int i = -1; i <= 1; i++) {
		    for(int j = -1; j <= 1; j++) {
                vec4 curr = texture(textureImage, textureCoord + vec2(i, j - 1) * texelSize);
                float gray = 0.21 * curr.r + 0.71 * curr.g + 0.07 * curr.b;
			    dx += curr.x * gx[3 * (i + 1) + (j + 1)];
		    }
	    }

	    for(int i = -1; i <= 1; i++) {
		    for(int j = -1; j <= 1; j++) {
                vec4 curr = texture(textureImage, textureCoord + vec2(i, j - 1) * texelSize);
                float gray = 0.21 * curr.r + 0.71 * curr.g + 0.07 * curr.b;
			    dy += curr.x * gy[3 * (i + 1) + (j + 1)];
		    }
	    }

        da = sqrt(dx*dx + dy*dy);


        for(int i = -1; i <= 1; i++) {
		    for(int j = -1; j <= 1; j++) {
                vec4 curr = texture(textureImage, textureCoord + vec2(i, j + 1) * texelSize);
                float gray = 0.21 * curr.r + 0.71 * curr.g + 0.07 * curr.b;
			    dx += curr.x * gx[3 * (i + 1) + (j + 1)];
		    }
	    }

	    for(int i = -1; i <= 1; i++) {
		    for(int j = -1; j <= 1; j++) {
                vec4 curr = texture(textureImage, textureCoord + vec2(i, j + 1) * texelSize);
                float gray = 0.21 * curr.r + 0.71 * curr.g + 0.07 * curr.b;
			    dy += curr.x * gy[3 * (i + 1) + (j + 1)];
		    }
	    }

         db = sqrt(dx*dx + dy*dy);
         if (da > dc || db > dc) {
            dc = 0;
         } 

       

    } else if (dir == 2) {

        float da = 0, db = 0;
        for(int i = -1; i <= 1; i++) {
		    for(int j = -1; j <= 1; j++) {
                vec4 curr = texture(textureImage, textureCoord + vec2(i + 1, j + 1) * texelSize);
                float gray = 0.21 * curr.r + 0.71 * curr.g + 0.07 * curr.b;
			    dx += curr.x * gx[3 * (i + 1) + (j + 1)];
		    }
	    }

	    for(int i = -1; i <= 1; i++) {
		    for(int j = -1; j <= 1; j++) {
                vec4 curr = texture(textureImage, textureCoord + vec2(i + 1, j + 1) * texelSize);
                float gray = 0.21 * curr.r + 0.71 * curr.g + 0.07 * curr.b;
			    dy += curr.x * gy[3 * (i + 1) + (j + 1)];
		    }
	    }

        da = sqrt(dx*dx + dy*dy);


        for(int i = -1; i <= 1; i++) {
		    for(int j = -1; j <= 1; j++) {
                vec4 curr = texture(textureImage, textureCoord + vec2(i - 1, j - 1) * texelSize);
                float gray = 0.21 * curr.r + 0.71 * curr.g + 0.07 * curr.b;
			    dx += curr.x * gx[3 * (i + 1) + (j + 1)];
		    }
	    }

	    for(int i = -1; i <= 1; i++) {
		    for(int j = -1; j <= 1; j++) {
                vec4 curr = texture(textureImage, textureCoord + vec2(i - 1, j - 1) * texelSize);
                float gray = 0.21 * curr.r + 0.71 * curr.g + 0.07 * curr.b;
			    dy += curr.x * gy[3 * (i + 1) + (j + 1)];
		    }
	    }

         db = sqrt(dx*dx + dy*dy);
         if (da > dc || db > dc) {
            dc = 0;
         } 
    
    } else if (dir == 3) {

        float da = 0, db = 0;
        for(int i = -1; i <= 1; i++) {
		    for(int j = -1; j <= 1; j++) {
                vec4 curr = texture(textureImage, textureCoord + vec2(i + 1, j) * texelSize);
                float gray = 0.21 * curr.r + 0.71 * curr.g + 0.07 * curr.b;
			    dx += curr.x * gx[3 * (i + 1) + (j + 1)];
		    }
	    }

	    for(int i = -1; i <= 1; i++) {
		    for(int j = -1; j <= 1; j++) {
                vec4 curr = texture(textureImage, textureCoord + vec2(i + 1, j) * texelSize);
                float gray = 0.21 * curr.r + 0.71 * curr.g + 0.07 * curr.b;
			    dy += curr.x * gy[3 * (i + 1) + (j + 1)];
		    }
	    }

        da = sqrt(dx*dx + dy*dy);


        for(int i = -1; i <= 1; i++) {
		    for(int j = -1; j <= 1; j++) {
                vec4 curr = texture(textureImage, textureCoord + vec2(i - 1, j) * texelSize);
                float gray = 0.21 * curr.r + 0.71 * curr.g + 0.07 * curr.b;
			    dx += curr.x * gx[3 * (i + 1) + (j + 1)];
		    }
	    }

	    for(int i = -1; i <= 1; i++) {
		    for(int j = -1; j <= 1; j++) {
                vec4 curr = texture(textureImage, textureCoord + vec2(i - 1, j) * texelSize);
                float gray = 0.21 * curr.r + 0.71 * curr.g + 0.07 * curr.b;
			    dy += curr.x * gy[3 * (i + 1) + (j + 1)];
		    }
	    }

         db = sqrt(dx*dx + dy*dy);
         if (da > dc || db > dc) {
            dc = 0;
         }
    
    } else {

        float da = 0, db = 0;
        for(int i = -1; i <= 1; i++) {
		    for(int j = -1; j <= 1; j++) {
                vec4 curr = texture(textureImage, textureCoord + vec2(i + 1, j - 1) * texelSize);
                float gray = 0.21 * curr.r + 0.71 * curr.g + 0.07 * curr.b;
			    dx += curr.x * gx[3 * (i + 1) + (j + 1)];
		    }
	    }

	    for(int i = -1; i <= 1; i++) {
		    for(int j = -1; j <= 1; j++) {
                vec4 curr = texture(textureImage, textureCoord + vec2(i + 1, j - 1) * texelSize);
                float gray = 0.21 * curr.r + 0.71 * curr.g + 0.07 * curr.b;
			    dy += curr.x * gy[3 * (i + 1) + (j + 1)];
		    }
	    }

        da = sqrt(dx*dx + dy*dy);


        for(int i = -1; i <= 1; i++) {
		    for(int j = -1; j <= 1; j++) {
                vec4 curr = texture(textureImage, textureCoord + vec2(i - 1, j + 1) * texelSize);
                float gray = 0.21 * curr.r + 0.71 * curr.g + 0.07 * curr.b;
			    dx += curr.x * gx[3 * (i + 1) + (j + 1)];
		    }
	    }

	    for(int i = -1; i <= 1; i++) {
		    for(int j = -1; j <= 1; j++) {
                vec4 curr = texture(textureImage, textureCoord + vec2(i - 1, j + 1) * texelSize);
                float gray = 0.21 * curr.r + 0.71 * curr.g + 0.07 * curr.b;
			    dy += curr.x * gy[3 * (i + 1) + (j + 1)];
		    }
	    }

         db = sqrt(dx*dx + dy*dy);
         if (da > dc || db > dc) {
            dc = 0;
         }
    }
    

    return vec4(dc);
    //return vec4(sqrt(dx*dx + dy*dy));
}

vec4 roberts()
{
    return vec4(0);
}

vec4 prewitt()
{
    return vec4(0);
}


void main()
{
    switch (outputMode)
    {
//        case 1:
//        {
//            out_color = grayscale();
//            break;
//        }
//
//        case 2:
//        {
//            out_color = blur(3);
//            break;
//        }
//
//        case 4:
//        {
//            out_color = median();
//            break;
//        }
//        
//        case 5:
//        {
//            out_color = sobel();
////            if (out_color.x < 0.2)
////                   out_color = vec4(0);
////            else
////                out_color = vec4(1);
//            break;
//        }
//
//        case 6:
//        {
//            out_color = roberts();
//            break;
//        }
//
//        case 7:
//        {
//            out_color = prewitt();
//            break;
//        }


        default:
            out_color = texture(textureImage, textureCoord);
            break;
    }
}
