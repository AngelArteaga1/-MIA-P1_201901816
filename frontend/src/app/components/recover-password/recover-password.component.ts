import { Component, OnInit } from '@angular/core';
import { Router,ActivatedRoute } from '@angular/router';
import { Recovery } from 'src/app/models/Recovery/recovery';

@Component({
  selector: 'app-recover-password',
  templateUrl: './recover-password.component.html',
  styleUrls: ['./recover-password.component.css']
})
export class RecoverPasswordComponent implements OnInit {

  constructor(private _Activatedroute:ActivatedRoute, private router: Router) { }

  sub:any;
  id = '';
  recoveryModel = new Recovery('', '');
  errorMessage = "";
  error = false;

  ngOnInit(): void {
    this.sub=this._Activatedroute.paramMap.subscribe(params => { 
      this.id = params.get('id') || ''; 
    });
  }

  public isNumber(char: string): boolean{
    if(char == "0") { return true; }
    if(char == "1") { return true; }
    if(char == "2") { return true; }
    if(char == "3") { return true; }
    if(char == "4") { return true; }
    if(char == "5") { return true; }
    if(char == "6") { return true; }
    if(char == "7") { return true; }
    if(char == "8") { return true; }
    if(char == "9") { return true; }
    return false;
  }

  public isSymbol(char: string): boolean{
    return !(/^[0-9a-zA-Z]+$/.test(char));
  }

  public hasUpperCase(password: string): boolean{
    for(let i = 0; i < password.length; i++){
      if(this.isNumber(password.charAt(i))){
        continue;
      }
      if(password.charAt(i).toUpperCase() === password.charAt(i)){
        return true;
      }
    }
    return false;
  }

  public hasNumber(password: string): boolean{
    for(let i = 0; i < password.length; i++){
      if(this.isNumber(password.charAt(i))){
        return true;
      }
    }
    return false;
  }

  public hasSymbol(password: string): boolean{
    for(let i = 0; i < password.length; i++){
      if(this.isSymbol(password.charAt(i))){
        return true;
      }
    }
    return false;
  }

  public validatePassword(password: string): boolean{
    if(password.length < 8){
      this.errorMessage = "Error: El campo contraseña tiene que tener minimo 8 caracteres"
      this.error = true;
      return false;
    }
    if(!this.hasUpperCase(password)){
      this.errorMessage = "Error: El campo contraseña tiene que tener minimo un caracter en mayuscula"
      this.error = true;
      return false;
    }
    if(!this.hasNumber(password)){
      this.errorMessage = "Error: El campo contraseña tiene que tener minimo un caracter de numero"
      this.error = true;
      return false;
    }
    /*if(!this.hasSymbol(password)){
      this.errorMessage = "Error: El campo contraseña tiene que tener minimo un caracter de simbolo"
      this.error = true;
      return false;
    }*/
    return true;
  }

  validate():boolean{
    if(this.recoveryModel.password == ""){
      this.errorMessage = "Error: El campo contraseña es obligatorio"
      this.error = true;
      return false;
    }
    if(this.recoveryModel.passwordConfirm == ""){
      this.errorMessage = "Error: El campo confirmacion de contraseña es obligatorio"
      this.error = true;
      return false;
    }
    if(!this.validatePassword(this.recoveryModel.password)){
      return false;
    }
    if(this.recoveryModel.password != this.recoveryModel.passwordConfirm){
      this.errorMessage = "Error: El campo de confirmar contraseña no es el mismo que el de contraseña"
      this.error = true;
      return false;
    }
    return true;
  }

  onSubmit(){
    //Validamos
    if(!this.validate()){ return; }
    //Otenemos el listado de usuarios
    let data = JSON.parse(localStorage.getItem("usuarios") || '{}');
    //Tenemos que iterar la lista
    let username = this.id;
    for(let i = 0; i < data.length; i++){
      //Tenemos que verificar si existe el usuario
      let user = data[i];
      if(user.username == username && user.status == true){
        //Tenemos que encontrar que hayan pasado solo 5 min desde que se mando el correo
        let today = new Date();
        let dateEmail = new Date(data[i].recover);
        if(today <= dateEmail){
          //Cambiamos la contraseña por la nueva
          data[i].password = this.recoveryModel.password;
          //Actualizamos el listado de usuarios
          localStorage.setItem('usuarios', JSON.stringify(data));
          //Redireccionamos a la ventada de confirmacion
          this.router.navigate(['/recoverConfirm']);
        } else {
          this.errorMessage = "Error: Ya vencio el tiempo para cambiar la contraseña."
          this.error = true;
          return;
        }
      }
    }
  }

}
