/*Graphics Lab2*/
#include <SFML/Graphics.hpp>
#include <SFML\OpenGL.hpp>

int main(int argc, char *argv)
{
  sf::RenderWindow window(sf::VideoMode(400, 400), "Lab2", sf::Style::Titlebar | sf::Style::Close);

  sf::Image questionBlock;
  questionBlock.loadFromFile("res/marioQBlock.jpg");

  enum keyDirection { stop, up, right, down, left };

  int modeIndex = 0; //for selection of drawing mode 
  int direction = stop; //for shape rotation
  int lastDirection = stop; //to remember the direction when direction key is released

  //for draw mode switching
  GLenum mode[] = {
  GL_QUADS,
  GL_TRIANGLES,
  GL_TRIANGLE_STRIP,
  GL_QUAD_STRIP
  };

  //for changing titles on the console window
  char *titleArray[] = {
	  "Lab2-Quads",
	  "Lab2-Triangles",
	  "Lab2-Triangles_Strip",
	  "Lab2-Quad_Strip"
  };

  // Tell OpenGL that we want to use 2D textures. 
  glEnable(GL_TEXTURE_2D);

  // Tell OpenGL that we want to use lighting.
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);


  /* Tell OpenGL that we want to test the depth of each vertex to make sure that things that
   * are closer appear closer. */
  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);
  glDepthFunc(GL_LESS);
  
  glEnable(GL_CULL_FACE);

  glFrontFace(GL_CW);
  // Generate a texture in OpenGL, and retrieve its ID so that we can access it.
  GLuint questionTextureID;
  glGenTextures(1, &questionTextureID);

  /* 'Bind' our texture to the active texture space so that all of our operations
   * are done to it. */
  glBindTexture(GL_TEXTURE_2D, questionTextureID);
  glTexImage2D(GL_TEXTURE_2D, // specifies that we're working on the active texture.
    0, // ignore.
    GL_RGBA, // specifies that making a texture that has RGBA data.
    questionBlock.getSize().x, // specify the width of the texture.
    questionBlock.getSize().y, // specify the height of the texture.
    0, // no border
    GL_RGBA, // specifies that we're working with an image that has RGBA data.
    GL_UNSIGNED_BYTE, // specifies the format of the RGBA data (in this case, everything is 0-255)
    questionBlock.getPixelsPtr() // specifies the actual image data.
    );
	
  /* Telling OpenGL that we want our active texture to magnify and minimize in 
   * linear fashion. */
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  /* Telling OpenGL how we want to represent our texture when we try to draw past its bounds. */
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  float positionZ = 0.5f;
  float angle = 0.0f;
  float textureAngle = 0.0f;
  float lastAngle = 0.0f;

  sf::Event evt;
  sf::Clock appTimer;

  while (window.isOpen()) {
    
	while (window.pollEvent(evt)) {
		if (evt.type == sf::Event::Closed) {
			window.close();
		}

		//Respond to key pressed events
		if (evt.type == sf::Event::EventType::KeyPressed) {
			
			if (evt.key.code == sf::Keyboard::Num1) {
				modeIndex = 0;
				SetConsoleTitle(titleArray[modeIndex]);
			}
			else if (evt.key.code == sf::Keyboard::Num2) {
				modeIndex = 1;
				SetConsoleTitle(titleArray[modeIndex]);
			}
			else if (evt.key.code == sf::Keyboard::Num3) {
				modeIndex = 2;
				SetConsoleTitle(titleArray[modeIndex]);
			}
			else if (evt.key.code == sf::Keyboard::Num4) {
				modeIndex = 3;
				SetConsoleTitle(titleArray[modeIndex]);
			}

			//record directional key presses
			else if (evt.key.code == sf::Keyboard::Right) 
				direction = right;		
			else if (evt.key.code == sf::Keyboard::Left)
				direction = left;
			else if (evt.key.code == sf::Keyboard::Up)
				direction = up;
			else if (evt.key.code == sf::Keyboard::Down)
				direction = down;					
		}

		//directional key release
		if (evt.type == sf::Event::EventType::KeyReleased) {
			if (evt.key.code == sf::Keyboard::Right
				|| evt.key.code == sf::Keyboard::Left
				|| evt.key.code == sf::Keyboard::Up
				|| evt.key.code == sf::Keyboard::Down)
				direction = stop;
		}
	}

	float deltaTime = appTimer.restart().asSeconds();
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLenum error = glGetError();

	float offset = 0.25f;

	//draw stuff
	GLfloat position[] = { positionZ, -0.1, positionZ, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glPushMatrix();

	//rotate texture
	textureAngle += 20.0f * deltaTime;
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glTranslatef(0.5f, 0.5f, 0.0f);
	glRotatef(textureAngle, 0.0f, 1.0f, 0.0f);
	glTranslatef(-0.5f, -0.5f, 0.0f);
	glMatrixMode(GL_MODELVIEW);
	
	//rotate cube based on direction key
	switch (direction) {  
		case right:		
			angle += 40.0f * deltaTime;						
			glRotatef(angle, 0.0f, 1.0f, 0.0f); 
			lastAngle = angle;
			lastDirection = direction;
			break;

		case left:
			angle -= 40.0f * deltaTime;
			glRotatef(angle, 0.0f, 1.0f, 0.0f); 
			lastAngle = angle;
			lastDirection = direction;
			break;

		case up:
			angle -= 40.0f * deltaTime;
			glRotatef(angle, 1.0f, 0.0f, 0.0f); 
			lastAngle = angle;
			lastDirection = direction;
			break;

		case down:
			angle += 40.0f * deltaTime;
			glRotatef(angle, 1.0f, 0.0f, 0.0f); 
			lastAngle = angle;
			lastDirection = direction;
			break;
	   
	   //remember last rotation
		case stop:
			switch (lastDirection) {
			case right:
				glRotatef(lastAngle, 0.0f, 1.0f, 0.0f);
				break;

			case left:
				glRotatef(lastAngle, 0.0f, 1.0f, 0.0f);
				break;

			case up:
				glRotatef(lastAngle, 1.0f, 0.0f, 0.0f);
				break;

			case down:
				glRotatef(lastAngle, 1.0f, 0.0f, 0.0f);
				break;
			}
		  break;
	}

      // Begin our drawing block.
      glBegin(mode[modeIndex]);

	  //for four sides other than top/bottom
        float sidesLeftS = 0.0f;
        float sidesRightS = 0.33f;
        float sidesTopT = 0.0f;
        float sidesBottomT = 1.0f;

		//for top
		float topLeftS = 0.68f;
		float topRightS = 1.0f;
		float topTopT = 0.0f;
		float topBottomT = 1.0f;

		//for bottom
		float botLeftS = 0.37f;
		float botRightS = 0.67f;
		float botTopT = 0.0f;
		float botBottomT = 1.0f;

        // Front
        glColor3f(1.0f, 0.0f, 0.0f);
        glTexCoord2f(sidesLeftS, sidesBottomT);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(-offset, -offset, offset);

        glTexCoord2f(sidesLeftS, sidesTopT);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(-offset, offset, offset);

        glTexCoord2f(sidesRightS, sidesTopT);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(offset, offset, offset);

        glTexCoord2f(sidesRightS, sidesBottomT);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(offset, -offset, offset);

        // Right
        glColor3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(sidesLeftS, sidesBottomT);
        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(offset, -offset, offset);

        glTexCoord2f(sidesLeftS, sidesTopT);
        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(offset, offset, offset);

        glTexCoord2f(sidesRightS, sidesTopT);
        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(offset, offset, -offset);

        glTexCoord2f(sidesRightS, sidesBottomT);
        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(offset, -offset, -offset);

        // Left
        glColor3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(sidesLeftS, sidesBottomT);
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(-offset, -offset, -offset);

        glTexCoord2f(sidesLeftS, sidesTopT);
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(-offset, offset, -offset);

        glTexCoord2f(sidesRightS, sidesTopT);
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(-offset, offset, offset);

        glTexCoord2f(sidesRightS, sidesBottomT);
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(-offset, -offset, offset);

        // Back
        glColor3f(0.0f, 1.0f, 1.0f);
        glTexCoord2f(sidesLeftS, sidesBottomT);
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(offset, -offset, -offset);

        glTexCoord2f(sidesLeftS, sidesTopT);
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(offset, offset, -offset);

        glTexCoord2f(sidesRightS, sidesTopT);
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(-offset, offset, -offset);

        glTexCoord2f(sidesRightS, sidesBottomT);
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(-offset, -offset, -offset);
	
        // Top
        glColor3f(1.0f, 0.0f, 1.0f);
        glTexCoord2f(topLeftS, topBottomT);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-offset, offset, offset);

        glTexCoord2f(topLeftS, topTopT);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-offset, offset, -offset);

        glTexCoord2f(topRightS, topTopT);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(offset, offset, -offset);

        glTexCoord2f(topRightS, topBottomT);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(offset, offset, offset);

        // Bottom
        glColor3f(1.0f, 1.0f, 1.0f);
        glTexCoord2f(botLeftS, botBottomT);
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(-offset, -offset, -offset);

        glTexCoord2f(botLeftS, botTopT);
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(-offset, -offset, offset);

        glTexCoord2f(botRightS, botTopT);
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(offset, -offset, offset);

        glTexCoord2f(botRightS, botBottomT);
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(offset, -offset, -offset);

      // End our drawing block.
      glEnd();

    glPopMatrix();

    window.display();
  }

  return 0;
}