import { Component } from '@angular/core';
import { HttpClient, JsonpClientBackend } from '@angular/common/http';
import { Observable, of } from 'rxjs';
import { faUserAstronaut } from '@fortawesome/free-solid-svg-icons'
import { faTree } from '@fortawesome/free-solid-svg-icons'
import { faRightFromBracket } from '@fortawesome/free-solid-svg-icons'
import { faListCheck} from '@fortawesome/free-solid-svg-icons'
import { Router } from '@angular/router';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent {
  title = 'frontend';
  faUserAstronaut = faUserAstronaut;
  faTree = faTree;
  faRightFromBracket =faRightFromBracket;
  faListCheck = faListCheck;
  username = '';

  constructor(private http: HttpClient, private router: Router){
    this.grabar_localstorage();
  }

  private _jsontreeURL = 'assets/files/tree.json';
  private _jsonURL = 'assets/files/users.json';


  public getJSON(): Observable<any> {
    return this.http.get(this._jsonURL);
  }

  public getTreeJSON(): Observable<any> {
    return this.http.get(this._jsontreeURL);
  }
  
  grabar_localstorage(){
    if(localStorage.getItem('usuarios') === null){
      //Tenemos que leer los usuarios del json
      this.getJSON().subscribe(data => {
        //Almacenamos la lista
        localStorage.setItem('usuarios', JSON.stringify(data));
      });
    }
    if(localStorage.getItem('tree') === null){
      //Tenemos que leer el tree del json
      this.getTreeJSON().subscribe(data => {
        //Almacenamos la lista
        localStorage.setItem('tree', JSON.stringify(data));
      });
    }
    
    if(localStorage.getItem('logeado')=== null){
      //Seteamos la variable de logeo
      localStorage.setItem('logeado', 'false');
    }
    //Seteamos el username
    if(localStorage.getItem('user') !== null){
      let user = JSON.parse(localStorage.getItem('user') || '{}');
      this.username = user.username;
    }
    //Seteamos los intentos de ingreso
    if(localStorage.getItem('intentos') === null){
      localStorage.setItem('intentos', '0');
    }
  }

  logout(){
    //logout
    localStorage.setItem('logeado', 'false');
    //redirect
    this.router.navigate(['/login']);
  }

}
