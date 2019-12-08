class GrassBlock{
    meshes = []
    colliders = []

    TAG = "GROUND"
    constructor(position){
        this.position = position;
        this.initializeObject();
    }

    initializeObject(){

        var texture = new THREE.TextureLoader().load( 'textures/Grass.jpg' );
        var geometry = new THREE.BoxGeometry(1, 1, 1 );
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
        this.mesh.position.set(this.position.x+0.5, this.position.y, this.position.z);
        this.mesh.receiveShadow = true;
        this.meshes.push(this.mesh)


        // Colliders
        this.mesh.geometry.computeBoundingBox();
        this.boundingBox = this.mesh.geometry.boundingBox.clone();


        this.boxHelper = new THREE.BoxHelper( this.mesh, 0xffff00 );
        this.colliders.push(this.boxHelper);        
    }
    render(){
        
    }
}