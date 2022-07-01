import { Component, OnInit } from '@angular/core';
import { Router, ActivatedRoute } from '@angular/router';
import { FileContent } from 'src/app/models/FileContent/file-content';
import { faFileLines } from '@fortawesome/free-solid-svg-icons'
import { Tree } from 'src/app/models/Tree/tree';
import jsPDF from 'jspdf';
import 'jspdf-autotable'

@Component({
  selector: 'app-file',
  templateUrl: './file.component.html',
  styleUrls: ['./file.component.css']
})
export class FileComponent implements OnInit {

  faFileLines = faFileLines;
  sub:any;
  path = [''];
  fileName = '';
  fileModel = new FileContent('');
  errorMessage = "";
  successMessage = "";
  parentPath='/directory/';
  error = false;
  success = false;

  constructor(private _Activatedroute:ActivatedRoute, private router: Router) { }

  ngOnInit(): void {
    //Verificando si ya esta logeado
    if(localStorage.getItem('logeado') == 'false'){this.router.navigate(['/login']);}
    //Ahora obtenemos el usuario
    this.sub=this._Activatedroute.paramMap.subscribe(params => { 
      //Seteamos el path
      this.path = (params.get('path')?.split('|')) || ['']; 
      this.path.shift();
      //Seteamos url del padre
      for(let i = 0; i < this.path.length-1; i++){
        this.parentPath += "|" + this.path[i];
      }
      if(this.parentPath == '/directory/'){
        this.parentPath = '/'
      }
      this.path.shift();
      //Seteamos el filename
      this.fileName = this.path[this.path.length-1];
      //Seteamos el content
      let arbolito = JSON.parse(localStorage.getItem('tree') || '{}') as Tree;
      this.setContent(arbolito, [...this.path]);
    });
  }

  setContent(nodo:Tree, path:string[]){
    let tmp = path.shift();
    for(let i = 0; i < nodo.sons.length; i++){
      if(path.length == 0){
        //Quiere decir que el file esta adentro
        if(nodo.sons[i].name == tmp && nodo.sons[i].type == '1'){
          this.fileModel.content = nodo.sons[i].content;
        }
      } else {
        //Quiere decir que faltan carpetas
        if(nodo.sons[i].name == tmp && nodo.sons[i].type == '0'){
          this.setContent(nodo.sons[i], path);
        }
      }
    }
  }

  saveContent(nodo:Tree, path:string[]){
    let tmp = path.shift();
    for(let i = 0; i < nodo.sons.length; i++){
      if(path.length == 0){
        //Quiere decir que el file esta adentro
        if(nodo.sons[i].name == tmp && nodo.sons[i].type == '1'){
          //Encontramos el nodo, actualizamos el contenido
          nodo.sons[i].content = this.fileModel.content;
        }
      } else {
        //Quiere decir que faltan carpetas
        if(nodo.sons[i].name == tmp && nodo.sons[i].type == '0'){
          this.saveContent(nodo.sons[i], path);
        }
      }
    }
  }

  onSubmit(){
    let arbolito = JSON.parse(localStorage.getItem('tree') || '{}') as Tree;
    this.saveContent(arbolito, [...this.path]);
    //Actualizamos el tree
    localStorage.setItem('tree', JSON.stringify(arbolito));
    //Mensaje de confirmacion
    this.success = true;
    this.successMessage = 'Archivo guardado correctamente';
  }

  generatePdfFile(){
    var pdf = new jsPDF();
    pdf.setFontSize(20);
    pdf.text(this.fileModel.content, 11, 8);
    pdf.save(this.fileName + '.pdf');
  }

  redirectTree(){
    this.router.navigate(['/']);
  }

}
