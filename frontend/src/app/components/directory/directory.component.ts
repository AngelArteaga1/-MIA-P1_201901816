import { Component, OnInit, Inject } from '@angular/core';
import { Router, ActivatedRoute } from '@angular/router';
import { faFolder, faFileLines } from '@fortawesome/free-solid-svg-icons'
import { Directory } from 'src/app/models/Directory/directory';
import { Tree } from 'src/app/models/Tree/tree';
export interface DialogData {
  animal: string;
  name: string;
}


@Component({
  selector: 'app-directory',
  templateUrl: './directory.component.html',
  styleUrls: ['./directory.component.css']
})
export class DirectoryComponent implements OnInit {

  blockName = '';
  faFolder = faFolder;
  faFileLines = faFileLines;
  sub:any;
  children: Directory[] = [];
  path = [''];
  stringPath= '';
  parentPath='/directory/';
  directoryName = '';


  constructor(private _Activatedroute:ActivatedRoute, 
    private router: Router) { }

  ngOnInit(): void {
    //Ahora obtenemos el usuario
    this.sub=this._Activatedroute.paramMap.subscribe(params => { 
      //Seteamos el path
      this.stringPath = params.get('path') || '';
      this.path = (this.stringPath.split('|')) || ['']; 
      this.path.shift();
      //Seteamos el filename
      this.directoryName = this.path[this.path.length-1];
      //Seteamos url del padre
      for(let i = 0; i < this.path.length-1; i++){
        this.parentPath += "|" + this.path[i];
      }
      if(this.parentPath == '/directory/'){
        this.parentPath = '/'
      }
      console.log(this.parentPath);
      //Seteamos los hijos
      let arbolito = JSON.parse(localStorage.getItem('tree') || '{}') as Tree;
      this.setChildren(arbolito, [...this.path]);
    });
  }

  setChildren(nodo:Tree, path:string[]){
    let tmp = path.shift();
    if(path.length == 0){
      //Deberiamos de estar dentro de nuestra carpeta
      if(tmp == nodo.name && nodo.type == '0'){
        for(let i = 0; i < nodo.sons.length; i++){
          let tmpPath = '';
          if(nodo.sons[i].type == '0'){
            tmpPath = '/directory/' + this.stringPath + "|" + nodo.sons[i].name; 
          } else {
            tmpPath = '/file/' + this.stringPath + "|" + nodo.sons[i].name; 
          }
          let son = new Directory(nodo.sons[i].name, tmpPath, nodo.sons[i].type )
          this.children.push(son);
        }
      }
      return;
    }
    //Quiere decir que tenemos que seguir iterando
    for(let i = 0; i < nodo.sons.length; i++){
      //Quiere decir que faltan carpetas
      if(nodo.sons[i].name == path[0] && nodo.sons[i].type == '0'){
        this.setChildren(nodo.sons[i], path);
      }
    }
  }

  createFile(){
    console.log(this.blockName);
    //Tenemos que ingresar el archivo con el nombre

    //Actualizamos la pagina
  }

  createDirectory(){
    console.log(this.blockName);
    //Tenemos que ingresar el archivo con el nombre

    //Actualizamos la pagina
  }

  redirectTree(){
    this.router.navigate(['/']);
  }

}