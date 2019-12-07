
class GameManager{

	isFogEnabled = false;

	gameStarted = true;
	camera;
	scene;
	renderer;
	gameObjects = [];
	busses = [];
	cars = [];
	logs = [];
	turtles = [];
	crabs = [];
	
	// Starting player position
	startingPlayerPosition = new THREE.Vector3(5,6,1);
	startingLives = 3;
	currentLives = 3;
	currentScore = 0;

	scorePerTarget = 200;

	MOVE_UP = 1;
	MOVE_DOWN = 0;

	// Scene properties
	scene_color = 0x000000;
	scene_color_alpha = 1;

	// Camera Properties
	camera_angle = 0;
	camera_range = -12;
	camera_speed = 0.5 * Math.PI/180;
	camera_target = new THREE.Vector3(5, 7, 0);
	camera_focal = 60;
	camera_near = 0.1;
	camera_far = 50;

	// Lights
	light_am_color = 0xAAAAAA;
	light_spot_color = 0xDDDDDD;
	light_spot_intensity = 0.7;
	light_spot_position = {x: 5, y: 10, z: 6}
	light_spot_camera_near = 0.5;
	light_spot_shadow_darkness = 0.35;

	// Fog
	fogColor = new THREE.Color(0xffffff);
	blackColor = new THREE.Color("rgb(0,0,0)");

	bus_speed = 0.1;
	car_speed = 0.06;
	log_speed = -0.06;
	turtle_speed = 0.06;

	possibleTargetPositions = [2,4,6,8];

	clippingPlanes = [new THREE.Plane( new THREE.Vector3( 1, 0, 0 )), new THREE.Plane( new THREE.Vector3( -1, 0, 0 ), 10)]

    constructor(){
	}
	cycle(){
		timeElapsed = new Date().getTime();
		timeDelta = timeElapsed - timePrevious;
		timePrevious = timeElapsed;
		requestAnimFrame(cycle);
		if(texturesLeft >0) return;
		update();
		renderScene();
	}

	initScene(){


	}
	render (currentTime) {
		if(!this.gameStarted) return;
		this.camera_angle += this.camera_speed;
		this.camera.position.x = Math.cos(this.camera_angle) * this.camera_range;
		this.camera.position.y = Math.sin(this.camera_angle) * this.camera_range;
		//this.camera.position.x = this.player.position.x;
		//this.camera.position.y = -5;
		this.camera.position.z = 5;
		this.camera.up.set(0, 0, 1);
		this.camera.lookAt(this.camera_target);
		//this.camera.lookAt(this.player.position);
		
		
		for(var obj of this.gameObjects){
			obj.render();
		}

		this.updateBusses();
		this.updateCars();
		this.updateLogs();
		this.updateTurtles(currentTime);
		
		var crabRotation = this.crabs[0].meshes[0].rotation;
		this.crabs[0].meshes[0].rotation.setFromRotationMatrix( this.camera.matrix );
		this.crabs[0].meshes[0].rotation.x = crabRotation.x;
		this.crabs[0].meshes[0].rotation.y = crabRotation.y;
		this.player.render(currentTime);

		var isCollidingWithBus = this.checkCollidingBusses();
		var isCollidingWithCar = this.checkCollidingCars();

		if(isCollidingWithCar || isCollidingWithBus)
			this.resetPlayer();

		this.checkGroundCollision();

		if(this.target.boundingBox.intersectsBox(this.player.boundingBox) || this.target.boundingBox2.intersectsBox(this.player.boundingBox)){
			this.respawnPlayer();
			this.currentScore += this.scorePerTarget;
		}
		this.displayText();
		this.renderer.render(this.scene, this.camera);
	};


	webGLStart(){


		// New renderer
		this.renderer = new THREE.WebGLRenderer({antialias: true});
		this.renderer.shadowMap.enabled = true;
		this.renderer.shadowMapType = THREE.PCFSoftShadowMap;
		this.renderer.setSize(window.innerWidth, window.innerHeight);
		this.renderer.setClearColor(this.scene_color, this.scene_color_alpha);

		// Add the renderer to the DOM
		document.body.appendChild(this.renderer.domElement);

		// Create the scene
		this.scene = new THREE.Scene();
 
		// ***** Clipping setup (renderer): *****
		this.renderer.localClippingEnabled = true;

		// ***** Fog *****
		this.toggleFog();

		// Set some camera defaults
		this.camera = new THREE.PerspectiveCamera(this.camera_focal, window.innerWidth/window.innerHeight,this.camera_near, this.camera_far);
		this.camera.position.set(0, this.camera_range, 0);
		this.camera.useQuaternion = true;
		this.camera.lookAt(this.camera_target);

		// Add abbient light
		var am_light = new THREE.AmbientLight(this.light_am_color); // soft white light
		this.scene.add(am_light);

		// LENS FLARE
		var textureLoader = new THREE.TextureLoader();

		var texture0 = textureLoader.load( "textures/lensflare/lensflare0.png" );
		var texture3 = textureLoader.load( "textures/lensflare/lensflare3.png" );

		this.lensflare = new THREE.Lensflare();
		this.lensflare.addElement( new THREE.LensflareElement( texture0, 300, 0 ) );
		this.lensflare.addElement( new THREE.LensflareElement( texture3, 60, 0.6 ) );
		this.lensflare.addElement( new THREE.LensflareElement( texture3, 70, 0.7 ) );
		this.lensflare.addElement( new THREE.LensflareElement( texture3, 120, 0.9 ) );
		this.lensflare.addElement( new THREE.LensflareElement( texture3, 70, 1 ) );

				
		// Add directional light
		this.spot_light = new THREE.SpotLight(this.light_spot_color, this.light_spot_intensity);
		this.spot_light.position.set(this.light_spot_position.x, this.light_spot_position.y, this.light_spot_position.z);
		this.spot_light.target = this.scene;
		this.spot_light.castShadow = true;
		this.spot_light.receiveShadow = true;
		this.spot_light.shadowDarkness = this.light_spot_shadow_darkness;
		this.spot_light.shadowCameraNear	= this.light_spot_camera_near;		

		this.spot_light.add( this.lensflare );

		this.scene.add(this.spot_light);

		this.gameObjects.push(new Grass(new THREE.Vector3(0,0,0)));
		this.gameObjects.push(new Grass(new THREE.Vector3(0,6,0)));
		this.gameObjects.push(new GrassBlock(new THREE.Vector3(0,12,0)));
		this.gameObjects.push(new GrassBlock(new THREE.Vector3(2,12,0)));
		this.gameObjects.push(new GrassBlock(new THREE.Vector3(4,12,0)));
		this.gameObjects.push(new GrassBlock(new THREE.Vector3(6,12,0)));
		this.gameObjects.push(new GrassBlock(new THREE.Vector3(8,12,0)));
		this.target = new Crystal(new THREE.Vector3(0,12,0));
		this.repositionTarget();
		this.gameObjects.push(this.target);
		this.gameObjects.push(new Road(new THREE.Vector3(0,3,0)));
		this.gameObjects.push(new River(new THREE.Vector3(0,9,0)));
		this.gameObjects.push(new River(new THREE.Vector3(0,9,0)));


		this.crabs.push(new Crab(new THREE.Vector3(0.5,12,0.8)));
		this.gameObjects.push(this.crabs[0]);
		this.player = new Frog(this.startingPlayerPosition);
		
		this.scene.add(this.player.meshes[0])
		this.scene.add(this.player.meshes[1])
		this.scene.add(this.player.meshes[2])


		this.createBusses();
		this.createCars();
		this.createLogs();
		this.createTurtles();
		for(var obj of this.gameObjects){
			for(var mesh of obj.meshes){
				this.scene.add(mesh);	
			}
		}

		
		// Cubemap
		var deception_pass = new THREE.CubeTextureLoader()
		.setPath( 'textures/' )
		.load( [
		'Road.jpg', 'Road.jpg',
		'Road.jpg', 'Road.jpg',
		'Road.jpg', 'Road.jpg'
		] );
		console.log(deception_pass)
		var cubemap = { type: 't', value: deception_pass }

		// Skybox
		

		var skyboxMaterial = new THREE.ShaderMaterial({
			uniforms: {
				skybox: { type: "t", value: cubemap },
			},
			side: THREE.DoubleSide
		});
		var loader = new THREE.FileLoader();
		loader.load('shaders/glsl/skybox.vs.glsl', function(shader) {
		skyboxMaterial.vertexShader = shader
		});
		loader.load('shaders/glsl/skybox.fs.glsl', function(shader) {
		skyboxMaterial.fragmentShader = shader
		});

		var skyboxGeometry = new THREE.BoxGeometry(100, 100,100);
		var skybox = new THREE.Mesh(skyboxGeometry, skyboxMaterial);
		this.scene.add(skybox)
		
		// Keyboard listener

		document.addEventListener("keydown", (event)=>{this.onDocumentKeyDown(event)}, false);

		// Render loop
		this.renderer.setAnimationLoop((currentTime)=>{this.render(currentTime);});
	}
	onDocumentKeyDown(event) {
		var keyCode = event.which;
		//console.log(keyCode)
		switch(keyCode){
			case 39:
				if(this.player.position.x < 9) this.player.currentDirection = this.player.direction.RIGHT;
				break;
			case 37:
				if(this.player.position.x > 0) this.player.currentDirection = this.player.direction.LEFT;
				break;
			case 38:
				if(this.player.position.y < 12) this.player.currentDirection = this.player.direction.UP;
				break;
			case 40:
				if(this.player.position.y > 0) this.player.currentDirection = this.player.direction.DOWN;
				break;
			case 82:
				this.resetPlayer();
				break;
			case 70: // fog
				this.isFogEnabled = !this.isFogEnabled;
				this.toggleFog();
				break;
		}
		
	};
	createBusses(){
		var offset = Math.random() * (5 - 0.1) + 0.1;
		this.busses.push(new Bus(new THREE.Vector3(-5,4,1),this.clippingPlanes));
		this.busses.push(new Bus(new THREE.Vector3(-8-offset,4,1),this.clippingPlanes));
		for(var bus of this.busses){
			this.gameObjects.push(bus);
		}
	}
	createCars(){
		this.cars.push(new Car(new THREE.Vector3(11,2,1), this.clippingPlanes));

		var offset = Math.random() * (2 - 0.1) + 0.1;
		this.cars.push(new Car(new THREE.Vector3(14+offset,2,1), this.clippingPlanes));
		
		var offset = Math.random() * (2 - 0.1) + 0.1;
		this.cars.push(new Car(new THREE.Vector3(17+offset,2,1), this.clippingPlanes));
		for(var car of this.cars){
			this.gameObjects.push(car);
		}
	}


	createLogs(){
		var offset = Math.random() * (2 - 0.1) + 0.1;
		this.logs.push(new Log(new THREE.Vector3(11+offset,11,1),this.clippingPlanes));

		var offset = Math.random() * (2 - 0.1) + 0.1;
		this.logs.push(new Log(new THREE.Vector3(14+offset,11,1),this.clippingPlanes));
		
		var offset = Math.random() * (2 - 0.1) + 0.1;
		this.logs.push(new Log(new THREE.Vector3(17+offset,11,1),this.clippingPlanes));


		var offset = Math.random() * (2 - 0.1) + 0.1;
		this.logs.push(new Log(new THREE.Vector3(11+offset,7,1)));

		var offset = Math.random() * (2 - 0.1) + 0.1;
		this.logs.push(new Log(new THREE.Vector3(14+offset,7,1),this.clippingPlanes));
		
		var offset = Math.random() * (2 - 0.1) + 0.1;
		this.logs.push(new Log(new THREE.Vector3(17+offset,7,1),this.clippingPlanes));


		var offset = Math.random() * (2 - 0.1) + 0.1;
		this.logs.push(new Log(new THREE.Vector3(11+offset,9,1)));

		var offset = Math.random() * (2 - 0.1) + 0.1;
		this.logs.push(new Log(new THREE.Vector3(14+offset,9,1),this.clippingPlanes));
		
		var offset = Math.random() * (2 - 0.1) + 0.1;
		this.logs.push(new Log(new THREE.Vector3(17+offset,9,1),this.clippingPlanes));

		for(var log of this.logs){
			this.gameObjects.push(log);
		}
	}

	repositionTarget(){


		var pos = this.possibleTargetPositions[Math.floor(Math.random()*this.possibleTargetPositions.length)];
		this.target.position = new THREE.Vector3(pos,12,0);
		this.target.updatePosition();
	}

	createTurtles(){
		this.turtles.push(new Turtle(new THREE.Vector3(-3,10,1),this.clippingPlanes));
		this.turtles.push(new Turtle(new THREE.Vector3(-4.5,10,1),this.clippingPlanes));
		this.turtles.push(new Turtle(new THREE.Vector3(-6,10,1),this.clippingPlanes));


		this.turtles.push(new Turtle(new THREE.Vector3(-9,10,1),this.clippingPlanes));
		this.turtles.push(new Turtle(new THREE.Vector3(-10.5,10,1),this.clippingPlanes));
		this.turtles.push(new Turtle(new THREE.Vector3(-12,10,1),this.clippingPlanes));


		var offset = Math.random() * (2 - 0.1) + 0.1;
		this.turtles.push(new Turtle(new THREE.Vector3(-3-offset,8,1),this.clippingPlanes));
		this.turtles.push(new Turtle(new THREE.Vector3(-4.5-offset,8,1),this.clippingPlanes));
		this.turtles.push(new Turtle(new THREE.Vector3(-6-offset,8,1),this.clippingPlanes));

		this.turtles.push(new Turtle(new THREE.Vector3(-9-offset,8,1),this.clippingPlanes));
		this.turtles.push(new Turtle(new THREE.Vector3(-10.5-offset,8,1),this.clippingPlanes));
		this.turtles.push(new Turtle(new THREE.Vector3(-12-offset,8,1),this.clippingPlanes));


		for(var turtle of this.turtles){
			this.gameObjects.push(turtle);
		}
	}
	checkCollidingBusses(){
		var isColliding = false;
		for(var bus of this.busses){
			isColliding = isColliding || bus.boundingBox.intersectsBox(this.player.boundingBox);
		}
		return isColliding;
	}
	checkCollidingCars(){
		var isColliding = false;
		for(var car of this.cars){
			isColliding = isColliding || car.boundingBox.intersectsBox(this.player.boundingBox);
		}
		return isColliding;
	}

	checkGroundCollision(){
		var hitRiver = false;
		var isOnGround = false;
		var isOnLog = false;
		var isOnTurtle = false;
		for(var gameObject of this.gameObjects){
			if(gameObject.TAG){
				var isColliding = this.player.bottomBoundingBox.intersectsBox(gameObject.boundingBox);
				if(isColliding){
					if(gameObject.TAG == "RIVER"){
						hitRiver = true;
					}else if(gameObject.TAG == "GROUND"){
						hitRiver = false;
						isOnLog = false;
						isOnGround = true;
						isOnTurtle = false;
					}else if(gameObject.TAG == "LOG"){
						isOnLog = true;
					}else if(gameObject.TAG == "TURTLE"){
						if(gameObject.isUnderWater){
							hitRiver = true;
							isOnTurtle = false;
						}else{
							hitRiver = false;
							isOnTurtle = true;
						}
					}
				}
			}
		}

		if(isOnLog){
			this.player.currentTag = "FLOATING";
			this.player.floatingMovingSpeed = this.log_speed;
		}else if(isOnTurtle){
			this.player.currentTag = "FLOATING";
			this.player.floatingMovingSpeed = this.turtle_speed;
		}else if (hitRiver && !isOnGround){
			this.resetPlayer();
		}else{
			this.player.currentTag = "GROUND";
		}

	}
	updateBusses(){
		for(var bus of this.busses){
			bus.position.x += this.bus_speed;
			if(bus.position.x > 12){
				bus.position.x = -4;
			}
			bus.updatePosition();
		}
	}

	updateCars(){
		for(var car of this.cars){
			car.position.x -= this.car_speed;
			if(car.position.x < -1){
				car.position.x = 11;
			}
			car.updatePosition();
		}
	}

	updateLogs(){
		for(var log of this.logs){
			log.position.x += this.log_speed;
			if(log.position.x < -1){
				log.position.x = 11;
			}
			log.updatePosition();
		}
	}


	updateTurtles(currentTime){
		for(var turtle of this.turtles){
			turtle.position.x += this.turtle_speed;
			if(turtle.position.x > 10){
				turtle.position.x = -3;
			}
			turtle.updatePosition(currentTime);
		}
	}
	respawnPlayer(){
		this.player.reset ();
		this.player.position = this.startingPlayerPosition;
	}
	resetPlayer(){
		this.currentLives -= 1;
		if(!this.gameStarted){
			this.respawnPlayer();
			this.currentLives = this.startingLives;
			this.gameStarted = true;
			this.score = 0;
		}

		if(this.currentLives <= 0){
			this.gameStarted = false;
		}else{
			this.respawnPlayer();
		}
	}

	toggleFog(){
		if(this.isFogEnabled){
			this.scene.background = this.fogColor;
			this.scene.fog = new THREE.Fog(this.fogColor, 0.0025, 20);			
		}else{
			this.scene.background = this.blackColor;
			this.scene.fog = null;
		}
	}
	displayText(){
		document.getElementById("score").innerHTML = "Score: " + this.currentScore;
		document.getElementById("lives").innerHTML = "Lives: " + this.currentLives;
	}
}