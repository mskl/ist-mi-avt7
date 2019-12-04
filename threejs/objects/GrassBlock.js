class GrassBlock{
    meshes = []
    constructor(position){
        this.position = position;
        this.initializeObject();
    }

    initializeObject(){

        var geometry = new THREE.BoxGeometry(1, 1, 1 );
        var material = new THREE.MeshLambertMaterial( { color: 0x00ff00 } );
        this.mesh = new THREE.Mesh( geometry, material );
        this.mesh.position.set(this.position.x+0.5, this.position.y, this.position.z);
        this.mesh.receiveShadow = true;
        this.meshes.push(this.mesh)
    }
    render(){
        
    }
}