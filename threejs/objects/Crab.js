class Crab{
    meshes = []
    constructor(position){
        this.position = position;
        this.initializeObject();
    }

    initializeObject(){

        var texture = new THREE.TextureLoader().load( 'textures/crab.png' );
		var geometry = new THREE.PlaneGeometry( 0.5, 0.5,20 );
		var material = new THREE.MeshBasicMaterial( {side: THREE.DoubleSide, map:texture, transparent: true} );
		this.plane = new THREE.Mesh( geometry, material );
		this.plane.position.x = this.position.x;
        this.plane.position.y = this.position.y;
        this.plane.position.z = this.position.z;
        this.meshes.push(this.plane)

    }
    render(){
        
    }
}