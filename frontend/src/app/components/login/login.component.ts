import { Component, OnInit } from '@angular/core';
import { Login } from 'src/app/models/Login/login';
import { Router } from '@angular/router';
import { HttpClient } from '@angular/common/http';
import { Observable } from 'rxjs';

@Component({
  selector: 'app-login',
  templateUrl: './login.component.html',
  styleUrls: ['./login.component.css']
})
export class LoginComponent implements OnInit {

  private _jsonURL = 'assets/files/users.json';

  constructor(private router: Router, private http: HttpClient) { 
  }

  loginModel = new Login('', '');
  errorMessage = "";
  error = false;

  ngOnInit(): void {
  }

  public getJSON(): Observable<any> {
    return this.http.get(this._jsonURL);
  }

  onSubmit(){
    //Tenemos que verificar si se encuentra el usuario
    this.getJSON().subscribe(data => {
      //Tenemos que iterar la lista
      for(let i = 0; i < data.length; i++){
        //Tenemos que verificar si existe el usuario
        let user = data[i];
        if(user.email == this.loginModel.email && user.password == this.loginModel.password){
            //Ingresamos el usuario en el localstorage
            localStorage.setItem("logeado", "true");
            localStorage.setItem("user", JSON.stringify(user));
            this.router.navigate(['/']);
        }
      }
      this.errorMessage = "Error: Usuario no encontrado :c"
      this.error = true;
      console.log(this.loginModel);
    });
  }

  redirectToRegister(){
    this.router.navigate(['/register']);
  }

}
