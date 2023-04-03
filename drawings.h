#pragma once
#ifndef DRAWINGS_H
#define DRAWINGS_H
/*Here are some drawings coordinates placed in const tabels*/
	//cactuses coordinates
		//drawing1
		static const float cactus_drawing1_x1[16] = { 40,35,25,25,5,0,0,5,10,15,80,75,65,65,65,65 };
		static const float cactus_drawing1_x2[16] = { 60,65,35,35,20,25,35,35,35,35,90,95,90,85,80,75 };
		static const float cactus_drawing1_y1[16] = { 0,5,160,165,45,50,100,105,110,115,40,45,100,105,110,115 };
		static const float cactus_drawing1_y2[16] = { 5,170,165,170,50,100,105,110,115,120,45,100,105,110,115,120 };
		static const float* cactus_drawing1[4] = { cactus_drawing1_x1 , cactus_drawing1_y1 , cactus_drawing1_x2 , cactus_drawing1_y2 };
		static const int cactus_drawing1_size = 16;
		//
	//
	// dino coordinates
		//drawing1
		static const float dino_drawing1_x1[28] = { 96, 86, 86, 86, 86, 80 ,70, 60, 150,50,0,0,0,0,0,0,10,20,30,40, /*left leg*/50,50, 50,50/**/,/*right leg*/90,100, 100,100 /**/ };
		static const float dino_drawing1_x2[28] = { 190, 200, 146, 180, 140,140, 160,140,160,140,30,20,10,10,140,130,130,120,110,110,/**/80,70,60,70/**/,/**/110,110,110,120 /**/ };
		static const float dino_drawing1_y1[28] = { 0,10, 60, 70 , 80, 90,100,110,110,120,120,110,100,90,130,140,150,160,170,180,/**/190,200,210,220/**/,/**/ 190,200,210,220/**/ };
		static const float dino_drawing1_y2[28] = { 10,60, 70, 80 , 90,100,110,120,120,130,130,120,110,100,140,150,160,170,180,190,/**/200,210,220,230/**/,/**/200,210,220,230 /**/ };
		static const float* dino_drawing1[4] = { dino_drawing1_x1 , dino_drawing1_y1 , dino_drawing1_x2 , dino_drawing1_y2 };
		static const int dino_drawing1_size = 28;
		//
		//drawing2
		static const float dino_drawing2_x1[26] = { 96, 86, 86, 86, 86, 80 ,70, 60, 150,50,0,0,0,0,0,0,10,20,30,40, /*left leg*/50,60/**/,/*right leg*/90,100, 100,100 /**/ };
		static const float dino_drawing2_x2[26] = { 190, 200, 146, 180, 140,140, 160,140,160,140,30,20,10,10,140,130,130,120,110,110,/**/70,80/**/,/**/110,110,110,120 /**/ };
		static const float dino_drawing2_y1[26] = { 0,10, 60, 70 , 80, 90,100,110,110,120,120,110,100,90,130,140,150,160,170,180,/**/190,200/**/,/**/ 190,200,210,220/**/ };
		static const float dino_drawing2_y2[26] = { 10,60, 70, 80 , 90,100,110,120,120,130,130,120,110,100,140,150,160,170,180,190,/**/200,210/**/,/**/200,210,220,230 /**/ };
		static const float* dino_drawing2[4] = { dino_drawing2_x1 , dino_drawing2_y1 , dino_drawing2_x2 , dino_drawing2_y2 };
		static const int dino_drawing2_size = 26;
		//
		//drawing3
		static const float dino_drawing3_x1[25] = { 96, 86, 86, 86, 86, 80 ,70, 60, 150,50,0,0,0,0,0,0,10,20,30,40, /*left leg*/50,50, 50,50/**/,/*right leg*/95/**/ };
		static const float dino_drawing3_x2[25] = { 190, 200, 146, 180, 140,140, 160,140,160,140,30,20,10,10,140,130,130,120,110,110,/**/80,70,60,70/**/,/**/115 /**/ };
		static const float dino_drawing3_y1[25] = { 0,10, 60, 70 , 80, 90,100,110,110,120,120,110,100,90,130,140,150,160,170,180,/**/190,200,210,220/**/,/**/ 190/**/ };
		static const float dino_drawing3_y2[25] = { 10,60, 70, 80 , 90,100,110,120,120,130,130,120,110,100,140,150,160,170,180,190,/**/200,210,220,230/**/,/**/200/**/ };
		static const float* dino_drawing3[4] = { dino_drawing3_x1 , dino_drawing3_y1 , dino_drawing3_x2 , dino_drawing3_y2 };
		static const int dino_drawing3_size = 25;
		//

		static const float** dino_drawings[3] = { dino_drawing1, dino_drawing3, dino_drawing2 };
		static const int dino_drawings_sizes[3] = { dino_drawing1_size,dino_drawing3_size ,dino_drawing2_size };
	//

	//menu point coordinates
	static const float menu_point_x1[3] = { 5, 0,5 };
	static const float menu_point_x2[3] = { 40,45,40 };
	static const float menu_point_y1[3] = { 0,5,25 };
	static const float menu_point_y2[3] = { 5,25,30 };
	static const float* menu_point_drawing[4] = { menu_point_x1 , menu_point_y1 , menu_point_x2 , menu_point_y2 };
	static const int menu_point_drawing_size = 3;
	//
/**/
#endif
