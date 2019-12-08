class Road{
    meshes = []
    colliders = []
    shapeSize = [10,5,1]
    TAG = "GROUND"
    constructor(position){
        this.position = position;
        this.initializeObject();
    }

    initializeObject(){
        var texture = new THREE.TextureLoader().load( 'textures/Road.jpg' );
        var geometry = new THREE.BoxGeometry(this.shapeSize[0],this.shapeSize[1],this.shapeSize[2]);
        var material = new THREE.MeshPhongMaterial( { map: texture } );
        var material2 = new THREE.MeshPhongMaterial( { color: "rgb(90,98,102)"} );
        var materials = [
            material2,        // Left side
            material2,       // Right side
            material2,         // Top side
            material2,      // Bottom side
            material,       // Front side
            material2         // Back side
        ];
        this.mesh = new THREE.Mesh( geometry, materials );
        this.mesh.position.set(this.position.x+this.shapeSize[0]/2, this.position.y, this.position.z);
        this.mesh.receiveShadow = true;
        this.meshes.push(this.mesh)


        // Colliders
        this.mesh.geometry.computeBoundingBox();
        this.boundingBox = this.mesh.geometry.boundingBox.clone();


        this.mesh.updateMatrixWorld( true );
        this.boundingBox.copy( this.mesh.geometry.boundingBox ).applyMatrix4( this.mesh.matrixWorld );

        this.boxHelper = new THREE.BoxHelper( this.mesh, 0xffff00 );
        this.colliders.push(this.boxHelper);        
    }

    render(){
        
    }
}