class Crystal{
    meshes = []
    colliders = []
    shapeSize = [2,0.7,0.1]
    rotationSpeed = 0.1;
    constructor(position){
        this.position = position;
        this.initializeObject();
    }

    initializeObject(){

        var geometry = new THREE.ConeGeometry( 0.25, 0.5, 6 );
        var material  = new THREE.MeshBasicMaterial( {color: "rgb(153,204,255)", transparent: true, opacity: 0.5} );
        this.mesh = new THREE.Mesh( geometry, material );
        this.mesh.rotation.y = 10;
        this.mesh.rotation.x = 3.14/2;
        this.mesh.receiveShadow = true;
        this.mesh.castShadow = true;
        this.meshes.push(this.mesh)

    
        this.mesh2 = new THREE.Mesh( geometry, material );
        this.mesh2.rotation.y = 10;
        this.mesh2.rotation.x = -3.14/2;
        this.mesh2.receiveShadow = true;
        this.mesh2.castShadow = true;
        this.meshes.push(this.mesh2)
        this.updatePosition();


        // Collider
        this.mesh.geometry.computeBoundingBox();
        this.boundingBox = this.mesh.geometry.boundingBox.clone();

        this.mesh2.geometry.computeBoundingBox();
        this.boundingBox2 = this.mesh2.geometry.boundingBox.clone();



        this.boxHelper = new THREE.BoxHelper( this.mesh, 0xffff00 );
        this.colliders.push(this.boxHelper);        

        this.boxHelper2 = new THREE.BoxHelper( this.mesh2, 0xffff00 );
        this.colliders.push(this.boxHelper2);        
    }
    render(){
        
        this.meshes[0].rotation.y += this.rotationSpeed;
        this.meshes[1].rotation.y -= this.rotationSpeed;


        // Update Collider
        this.mesh.updateMatrixWorld( true );
        this.boundingBox.copy( this.mesh.geometry.boundingBox ).applyMatrix4( this.mesh.matrixWorld );

        this.boxHelper.update();


        this.mesh2.updateMatrixWorld( true );
        this.boundingBox2.copy( this.mesh2.geometry.boundingBox ).applyMatrix4( this.mesh2.matrixWorld );

        this.boxHelper2.update();
    }

    updatePosition(){
        this.meshes[0].position.set(this.position.x+0.5, this.position.y, 1+0.5);
        this.meshes[1].position.set(this.position.x+0.5, this.position.y, 1+0);
    }
    
}