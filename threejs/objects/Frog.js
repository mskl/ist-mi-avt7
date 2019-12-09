class Frog{
    meshes = [];
    colliders = [];
    direction = {
        NONE: 0,
        UP: 1,
        DOWN: 2, 
        LEFT: 3 ,
        RIGHT: 4 
      };
    isJumping = false;
    currentDirection = this.direction.NONE;
    animationSpeed = 200;
    clock = new THREE.Clock();
    startAnimationPosition
    currentTag = "GROUND";

    floatingMovingSpeed = 0;
    constructor(position){
        this.position = position;
        //this.position.x += 0.5;
        this.initializeObject();
    }

    initializeObject(){

        var texture = new THREE.TextureLoader().load( 'textures/Grass.jpg' );
        var geometry = new THREE.BoxGeometry(0.5, 0.5, 0.5 );
        var materialTextured = new THREE.MeshLambertMaterial( { map: texture } );
        var materialColor = new THREE.MeshLambertMaterial( { color: 0x00ff00} );
        
        var materials = [
            materialColor,        // Left side
            materialColor,       // Right side
            materialTextured,         // Top side
            materialTextured,      // Bottom side
            materialTextured,       // Front side
            materialColor         // Back side
        ];


        var materialTemp = new THREE.MeshLambertMaterial( { transparent: true, opacity:0.0} );
        
        this.mesh = new THREE.Mesh( geometry, materials );
        this.meshBottomCollider = new THREE.Mesh( geometry, materialTemp );
        this.mesh.position.set(this.position.x, this.position.y, this.position.z);
        this.mesh.receiveShadow = true;
        this.meshes.push(this.mesh)
        this.meshes.push(this.meshBottomCollider)

        // Collider
        this.mesh.geometry.computeBoundingBox();
        this.boundingBox = this.mesh.geometry.boundingBox.clone();


        this.meshBottomCollider.geometry.computeBoundingBox();
        this.bottomBoundingBox = this.meshBottomCollider.geometry.boundingBox.clone();


        this.boxHelper = new THREE.BoxHelper( this.mesh, 0xffff00 );
        this.colliders.push(this.boxHelper);
    }
    render(currentTime){

        var newPosition = this.position.clone();

        if(this.currentTag == "FLOATING" && !this.isJumping && this.currentDirection == this.direction.NONE){
            newPosition.x += this.floatingMovingSpeed;
            console.log("Floating");
        }
        if(this.currentDirection != this.direction.NONE && !this.isJumping){
            this.finalPosition = this.position.clone();
            this.startPosition = this.position.clone();
            switch(this.currentDirection) {
                case this.direction.UP:
                    this.finalPosition.y += 1;
                    break;
                case this.direction.DOWN:
                    this.finalPosition.y += -1;
                    break;
                case this.direction.LEFT:
                    this.finalPosition.x += -1;
                    break;
                case this.direction.RIGHT:
                    this.finalPosition.x += 1;
                    break;    
                default:
                // code block
            }
            if(this.currentTag != "FLOATING"){
                this.finalPosition.x = Math.round(this.finalPosition.x);
                this.finalPosition.y = Math.round(this.finalPosition.y);
                this.finalPosition.z = Math.round(this.finalPosition.z);
            }
            this.startAnimationPosition = currentTime;
            this.isJumping = true;
        }

        if(this.isJumping){
            var alpha = (currentTime - this.startAnimationPosition)/this.animationSpeed;
            newPosition.z = 1 + Math.sin(alpha * 3.14) / 2.0; 
            newPosition.x = this.startPosition.x * (1 - alpha) + this.finalPosition.x*alpha; 
            newPosition.y = this.startPosition.y * (1 - alpha) + this.finalPosition.y*alpha; 
            if(alpha >= 1){
                this.isJumping = false;
                this.currentDirection = this.direction.NONE;
                newPosition = this.finalPosition.clone();
            }
        }

        this.position = newPosition.clone();

        this.updatePosition();

        // Update Collider
        this.mesh.updateMatrixWorld( true );
        this.boundingBox.copy( this.mesh.geometry.boundingBox ).applyMatrix4( this.mesh.matrixWorld );

        this.meshBottomCollider.updateMatrixWorld( true );
        this.bottomBoundingBox.copy( this.meshBottomCollider.geometry.boundingBox ).applyMatrix4( this.meshBottomCollider.matrixWorld );
        
        this.boxHelper.update();
    }

    updatePosition(){
        this.mesh.position.set(this.position.x+0.5, this.position.y,this.position.z-0.25);
        this.meshBottomCollider.position.set(this.position.x+0.5, this.position.y,this.position.z-0.75);
    }

    reset(){
        this.isJumping = false;
        this.currentDirection = this.direction.NONE;
    }
}