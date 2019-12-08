class Log{
    meshes = []
    colliders = []
    shapeSize = [2,0.7,0.1]
    clippingPlanes = [];
    TAG = "LOG";
    constructor(position, clippingPlanes){
        this.position = position;
        if(clippingPlanes != null){
            this.clippingPlanes = clippingPlanes;
        }
        this.initializeObject();
    }

    initializeObject(){

        var geometry = new THREE.BoxGeometry(this.shapeSize[0],this.shapeSize[1],this.shapeSize[2]);
        console.log(this.clippingPlanes)
        // Geometry
        var material = new THREE.MeshPhongMaterial( {
            shininess: 100,
            color: "rgb(150, 75, 0)",
            side: THREE.DoubleSide,
            clippingPlanes: this.clippingPlanes,
            clipShadows: true
        } );
        this.mesh = new THREE.Mesh( geometry, material );
        this.mesh.receiveShadow = true;
        this.mesh.castShadow = true;
        this.meshes.push(this.mesh)
        
        // Colliders
        this.mesh.geometry.computeBoundingBox();
        this.boundingBox = this.mesh.geometry.boundingBox.clone();


        this.boxHelper = new THREE.BoxHelper( this.mesh, 0xffff00 );
        this.colliders.push(this.boxHelper);

        this.updatePosition();

    }
    render(){
        

    }

    updatePosition(){
        this.mesh.position.set(this.position.x+this.shapeSize[0]/2, this.position.y, 0.4);

        
        this.mesh.updateMatrixWorld( true );
        this.boundingBox.copy( this.mesh.geometry.boundingBox ).applyMatrix4( this.mesh.matrixWorld );

        
        this.boxHelper.update();
    }
}