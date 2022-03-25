color background_color;
Boids boids;
Objects objects;
Boids ATONs;

ButtonsThatAreOneClick back_frame_button;
ButtonsThatAreOneClick next_frame_button;
ButtonsThatAreOneClick all_frames_button;
ButtonsThatAreOneClick show_ATONs_button;

String[] frameFiletxt = null;
int num_boids = 0;
int frames = 0;
int frame_index = 0;
int read_index = 0;
int dimension_size = 0;
int dim = 0;
int max_range_size = 0;
int frame_start = 11;
int num_objs = 0;
int num_ATONs = 0;
boolean draw_ATONs = false;

int max_colorRGB = 255;

void setup()
{
	size(100, 100);
	surface.setResizable(true); // allows resizing

	/* set color mode */
	colorMode(RGB, max_colorRGB); // max color is 100 R, G, B
	
	selectInput("Select a file to process:", "fileSelected");

	/* initialize the seed */
	randomSeed(mouseX + mouseY);
}

boolean run_once = false;
boolean all_frame_play = false;
void draw()
{
	int x_width = 600;
	int y_width = 600;
	int scale_factor = 1;
	int scale_size = 1;
  
	if ((run_once == false) && (frameFiletxt != null) && (frame_index != 0)) //<>//
	{
		background(0,0,0);
		if (dimension_size < x_width)
		{
			/* decide how to make the grid.  600 by 600 if grid size is less than 300 then magnify otherwise, size of grid */
			/* Set the size of the area */
			println("WARNING! large dimension haven't thought about.");
			surface.setSize(x_width, y_width+50);
			scale_factor = x_width / dimension_size;
			scale_size = x_width / dimension_size;
		}
		else 
		{
			surface.setSize(dimension_size, dimension_size+50);
		}
	
		/* initialize the grid */
		boids = new Boids(num_boids, scale_size, scale_factor);

		/* create GUI items */
		next_frame_button = new ButtonsThatAreOneClick(0, 600, 149, 50, color(0,255,0), color(255,255,255), "Next Frame");
		back_frame_button = new ButtonsThatAreOneClick(150, 600, 149, 50, color(0,0,255), color(255,255,255), "Back Frame");
		all_frames_button = new ButtonsThatAreOneClick(300, 600, 149, 50, color(255,0,0), color(255,255,255), "Play Frames");
    show_ATONs_button = new ButtonsThatAreOneClick(450, 600, 149, 50, color(0, 125, 125), color(255,255,255), "Show/Hide ATONs");
    
    println(show_ATONs_button.is_button_on());
    
		/* update grid with next frame of file */
		boids.read_next_frame();
		/* initial draw */
    objects.update_draw();
		boids.update_draw();
    if (draw_ATONs == true) {
      ATONs.update_draw();  
    }
		next_frame_button.button_draw();
		back_frame_button.button_draw();
		all_frames_button.button_draw();
    show_ATONs_button.button_draw();

		run_once = true;
		all_frame_play = false;
	}
	else if ((run_once == true) && (all_frame_play == false) && (next_frame_button.is_button_on() == true))
	{
		/* update grid with next frame of file */
		boids.read_next_frame();

    objects.update_draw();
		boids.update_draw();
    if (draw_ATONs == true) {
      ATONs.update_draw();  
    }
		next_frame_button.button_draw();
		back_frame_button.button_draw();
		all_frames_button.button_draw();
    show_ATONs_button.button_draw();

		/* turn off so we only get one click of it */
		next_frame_button.turn_button_off();
	}
	else if ((run_once == true) && (all_frame_play == false) && (back_frame_button.is_button_on() == true))
	{
		/* update grid with next frame of file */
		boids.read_back_frame();
    
    objects.update_draw();
		boids.update_draw();
    if (draw_ATONs == true) {
      ATONs.update_draw();  
    }
		next_frame_button.button_draw();
		back_frame_button.button_draw();
		all_frames_button.button_draw();
    show_ATONs_button.button_draw();
    
		/* turn off so we only get one click of it */
		back_frame_button.turn_button_off();
	}
  else if ((run_once == true) && (all_frame_play == false) && (show_ATONs_button.is_button_on() == true))
  {
    objects.update_draw();
    boids.update_draw();
    if (draw_ATONs == true) {
      ATONs.update_draw();  
    }
    next_frame_button.button_draw();
    back_frame_button.button_draw();
    all_frames_button.button_draw();
    show_ATONs_button.button_draw();
    show_ATONs_button.turn_button_off();
    draw_ATONs = !draw_ATONs;
    println(draw_ATONs);
  }
  else if ((run_once == true) && (all_frame_play == true) && (show_ATONs_button.is_button_on() == true))
  {
    objects.update_draw();
    boids.update_draw();
    if (draw_ATONs == true) {
      ATONs.update_draw();  
    }
    next_frame_button.button_draw();
    back_frame_button.button_draw();
    all_frames_button.button_draw();
    show_ATONs_button.button_draw();
    show_ATONs_button.turn_button_off();
    draw_ATONs = !draw_ATONs;
    println(draw_ATONs);
  }
	else if ((run_once == true) && (all_frame_play == false) && (all_frames_button.is_button_on() == true))
	{
    objects.update_draw();
		boids.update_draw();
    if (draw_ATONs == true) {
      ATONs.update_draw();  
    }
		next_frame_button.button_draw();
		back_frame_button.button_draw();
		all_frames_button.button_draw();
    show_ATONs_button.button_draw();
    
		/* turn off so we only get one click of it */
		all_frames_button.turn_button_off();
		all_frame_play = true;
	}
	else if ((run_once == true) && (all_frame_play == true))
	{
		/* keep on playing frames */
		boids.read_next_frame();

		if (frame_index == frames-1)
		{
			all_frame_play = false;
			frame_index = 1; // where the frames start
			read_index = frame_start + (num_objs * 4); // where the reading start
		}

		background(0,0,0);
    objects.update_draw();
		boids.update_draw();
    if (draw_ATONs == true) {
      ATONs.update_draw();  
    }
		next_frame_button.button_draw();
		back_frame_button.button_draw();
		all_frames_button.button_draw();
    show_ATONs_button.button_draw();
    
		/* turn off all until done */
		next_frame_button.turn_button_off();
		back_frame_button.turn_button_off();
		all_frames_button.turn_button_off();  
    show_ATONs_button.turn_button_off();
	}
	else if (run_once == true)
	{
		background(0,0,0);
    objects.update_draw();
		boids.update_draw();
    if (draw_ATONs == true) {
      ATONs.update_draw();  
    }
		next_frame_button.button_draw();
		back_frame_button.button_draw();
		all_frames_button.button_draw();
    show_ATONs_button.button_draw();
	}
}

void fileSelected(File selection) 
{
	if (selection == null) 
	{
		println("Window was closed or the user hit cancel.");
	} 
	else 
	{
		String filepath = selection.getAbsolutePath();
		println("User selected " + filepath);
		// load file here
		frameFiletxt = loadStrings(filepath);

		
		num_boids = int(frameFiletxt[0]);
		frames = int(frameFiletxt[1]);
		dimension_size = int(frameFiletxt[2]);
		dim = int(frameFiletxt[3]);
    num_objs = int(frameFiletxt[9]);
    num_ATONs = int(frameFiletxt[10]);
    read_index = frame_start; // where the reading start
    objects = new Objects(num_objs); 
    ATONs = new Boids(num_ATONs);
		frame_index = 1; // where the frames start //<>//
		println("frame_index "+frame_index);
	}
}

void mouseClicked()
{
	next_frame_button.update_clicked();
	back_frame_button.update_clicked();
	all_frames_button.update_clicked();
  show_ATONs_button.update_clicked();
	println(frame_index+":"+back_frame_button.is_button_on()+":"+next_frame_button.is_button_on()+":"+all_frames_button.is_button_on()+":"+show_ATONs_button.is_button_on()); 
}

class Boids
{
	Boid[] boids;
	int num_boids;
	int scale_size, scale_factor;

	Boids(int num_boids, int scale_size, int scale_factor) // square
	{
		this.num_boids = num_boids;
		this.scale_size = scale_size;
		this.scale_factor = scale_factor;

		boids = new Boid[num_boids];

		for (int i=0; i < num_boids; i++)
		{
			boids[i] = new Boid();
		}
	}

  Boids(int num_ATONs) { //used to initialize ATONs. Actual boids use the above constructor
    this.num_boids = num_ATONs;
    this.scale_size = -1;
    this.scale_factor = -1;
    
    boids = new Boid[num_ATONs];
    
    for (int i = 0; i < num_ATONs; i++) {
      boids[i] = new Boid(float(frameFiletxt[read_index]), float(frameFiletxt[read_index+1]), int(frameFiletxt[read_index+2]), int(frameFiletxt[read_index+3]));
      read_index += 4;
    }
  }
	void update_draw()
	{
		for (int i=0; i < this.num_boids; i++)
		{
			boids[i].update_draw();
		}
	}

	void read_next_frame()
	{
		int i;

		if (frame_index == frames-1)
		{
			return;
		}

		if (dim != 2)
			println("WARNING not Dimension 2");
		for (i = 0; i < this.num_boids; i++)
		{
			boids[i].update_position(float(frameFiletxt[read_index]), float(frameFiletxt[read_index+1]));
			boids[i].update_velocity(float(frameFiletxt[read_index+2]), float(frameFiletxt[read_index+3]));
			boids[i].update_life_status(int(frameFiletxt[read_index+4]));
      boids[i].update_leader_status(int(frameFiletxt[read_index+5]));
      read_index += 6;
		}
		frame_index ++;
	}

	void read_back_frame()
	{
		int i;

		if (frame_index == 1)
		{
			return;
		}

		/* move the reader to start of frame */
		read_index -= this.num_boids*4;

		if (dim != 2)
			println("WARNING not Dimension 2");
		for (i = 0; i < this.num_boids; i++)
		{
			boids[i].update_position(float(frameFiletxt[read_index]), float(frameFiletxt[read_index+1]));
			boids[i].update_velocity(float(frameFiletxt[read_index+2]), float(frameFiletxt[read_index+3]));
      boids[i].update_life_status(int(frameFiletxt[read_index+4]));
      boids[i].update_leader_status(int(frameFiletxt[read_index+5]));
			read_index += 6;
		}

		frame_index --;
	}
}

class Boid
{
  PVector position;
  PVector velocity;
  color boid_color;
  int life_status;
  int is_leader;

  Boid()
  {
    velocity = new PVector(0,0);
    position = new PVector(0,0);
    this.boid_color = color(255, 255, 255);
  }
  
  Boid(float p_x, float p_y, float v_x, float v_y) { //usually used only by ATONS, actual boid uses default constructor
    position = new PVector(p_x, p_y);
    velocity = new PVector(v_x, v_y);
    this.boid_color = color(256, 125, 125);
    this.life_status = 1;
    this.is_leader = 1;
  }

  void update_position(float x, float y)
  {
    position.set(x, y);
  }
  void update_velocity(float x, float y)
  {
    velocity.set(x, y);
  }

  void update_life_status(int s) 
  {
    life_status = s; 
  }
  
  void update_leader_status(int s) 
  {
    is_leader = s; 
    if (s == 0){
      boid_color = color(255, 255, 255); 
    } else {
      boid_color = color(0, 200, 30);
    }
  }
  
  void update_draw()
  {
    float theta = velocity.heading2D() + radians(90);
    float r = 3.0;
    if (life_status == 1) //alive
    {
      fill(boid_color);
    } else {
      fill(255, 45, 25);
    }
    stroke(0);
    pushMatrix();
    translate(position.x,position.y);
    rotate(theta);
    beginShape(TRIANGLES);
    vertex(0, -r*2);
    vertex(-r, r*2);
    vertex(r, r*2);
    endShape();
    popMatrix();
    noFill();
    stroke(255, 255, is_leader * 255);
    circle(position.x, position.y, 10);
  }
}

class ButtonsThatAreOneClick
{
	int x_top, y_top;
	int x_size, y_size;
	color button_color;
	color font_color;
	String button_text;
	boolean button_on;
	boolean over;

	ButtonsThatAreOneClick(int x_top, int y_top, int x_size, int y_size, color button_color, color font_color, String button_text)
	{
		this.x_top = x_top;
		this.y_top = y_top;
		this.x_size = x_size; 
		this.y_size = y_size;
		this.button_color = button_color;
		this.font_color = font_color;
		this.button_text= button_text;

		this.button_on = false;
	}

	boolean is_button_on()
	{
		return button_on;
	}
	void turn_button_off()
	{
		button_on = false;
	}

	boolean update_clicked() 
	{	
		if(is_over()) 
		{	
			over = true;	
		} 
		else 
		{	
			over = false;	
		}	

		if(over) 
		{	
			button_on = true;
		}

		button_draw();

		return button_on;
	}

	void button_draw()
	{
		/* create the rectangle button */
		fill(button_color);
		stroke(color(255,255,255));

		rect(x_top, y_top, x_size, y_size);

		/* put the writing on */
		fill(font_color);
		text(button_text, x_top+3, y_top+3, x_size, y_size);
	}

	boolean is_over()
	{	
		if(mouseX > x_top && mouseX < x_top+x_size &&	mouseY > y_top && mouseY < y_top+y_size) 
		{	
			return true;	
		} 
		else 
		{	
			return false;	
		} 
	}
}
class ButtonsThatToggleValues
{
	int x_top, y_top;
	int x_size, y_size;
	color button_color;
	color font_color;
	String button_text_on;
	String button_text_off;
	boolean button_on;
	boolean over;

	ButtonsThatToggleValues(int x_top, int y_top, int x_size, int y_size, color button_color, color font_color, String button_text_on, String button_text_off)
	{
		this.x_top = x_top;
		this.y_top = y_top;
		this.x_size = x_size; 
		this.y_size = y_size;
		this.button_color = button_color;
		this.font_color = font_color;
		this.button_text_on = button_text_on;
		this.button_text_off = button_text_off;

		this.button_on = false;
	}

	boolean is_button_on()
	{
		return button_on;
	}
	void turn_button_off()
	{
		button_on = false;
	}

	boolean update_clicked() 
	{	
		if(is_over()) 
		{	
			over = true;	
		} 
		else 
		{	
			over = false;	
		}	

		if(over) 
		{	
			if (this.button_on == true)
			{
				button_on = false;
			}
			else
			{
				button_on = true;
			}
		}

		button_draw();

		return button_on;
	}

	void button_draw()
	{
		/* create the rectangle button */
		fill(button_color);
		stroke(color(255,255,255));

		rect(x_top, y_top, x_size, y_size);

		/* put the writing on */
		fill(font_color);
		if (button_on)
		{
			text(button_text_on, x_top+3, y_top+3, x_size, y_size);
		}
		else
		{
			text(button_text_off, x_top+3, y_top+3, x_size, y_size);
		}
	}

	boolean is_over()
	{	
		if(mouseX > x_top && mouseX < x_top+x_size &&	mouseY > y_top && mouseY < y_top+y_size) 
		{	
			return true;	
		} 
		else 
		{	
			return false;	
		} 
	}
}
//=======================================================================================================

class Object
{
  PVector position;
  PVector velocity;
  int radius;
  color obj_color;
  int is_waypoint;
  
  Object(float x, float y, int r, int is_wp)
  {
    velocity = new PVector(0,0);
    position = new PVector(x,y);
    if (is_wp == 0) {
      this.obj_color = color(55, 255, 255);
    } else {
      this.obj_color = color(50, 255, 50);
    }
    radius = r;
    this.is_waypoint = is_wp;
  }
  
  void update_draw()
  {
    fill(obj_color);
    stroke(0);
    circle(position.x, position.y, 2 * radius);
  }
}

//=======================================================================================================

class Objects
{
  Object[] objs;
  int num_objs;
  
  Objects(int num_objs)
  {
    this.num_objs = num_objs;

    objs = new Object[this.num_objs];

    for (int i=0; i < this.num_objs; i++)
    {
      objs[i] = new Object(float(frameFiletxt[read_index]), float(frameFiletxt[read_index+1]), int(frameFiletxt[read_index+2]), int(frameFiletxt[read_index+3]));
      read_index += 4;
    }
  }
  
  void update_draw()
  {
    for (int i=0; i < this.num_objs; i++)
    {
      objs[i].update_draw();
    }
  }
}

//======================================================================================================
