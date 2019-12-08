class Grass{
    meshes = []
    colliders = []
    shapeSize = [10,1,1]
    TAG = "GROUND"
    constructor(position){
        this.position = position;
        this.initializeObject();
    }

    initializeObject(){

        var texture = new THREE.TextureLoader().load( 'textures/Grass.jpg' );
        var geometry = new THREE.BoxGeometry(this.shapeSize[0],this.shapeSize[1],this.shapeSize[2]);
        var materialTextured = new THREE.MeshPhongMaterial( { map: texture } );
        var materialColor = new THREE.MeshPhongMaterial( { color: 0x00ff00} );
        
        var materials = [
            materialColor,        // Left side
            materialColor,       // Right side
            materialTextured,         // Top side
            materialTextured,      // Bottom side
            materialTextured,       // Front side
            materialColor         // Back side
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