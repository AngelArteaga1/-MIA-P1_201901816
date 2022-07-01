import { Component, OnInit } from '@angular/core';
import { Register } from 'src/app/models/Register/register';
import { Router } from '@angular/router';
import '../../../assets/js/smtp.js';
declare let Email: any;

@Component({
  selector: 'app-register',
  templateUrl: './register.component.html',
  styleUrls: ['./register.component.css']
})
export class RegisterComponent implements OnInit {
  
  constructor(private router: Router) { }

  registerModel = new Register('', '', '', '', '', '', '', new Date(0), new Date(), new Date(0), false, false);
  errorMessage = "";
  error = false;

  ngOnInit(): void {
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

  public validateEmail(email: string){
    return String(email)
      .toLowerCase()
      .match(
        /^(([^<>()[\]\\.,;:\s@"]+(\.[^<>()[\]\\.,;:\s@"]+)*)|(".+"))@((\[[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\])|(([a-zA-Z\-0-9]+\.)+[a-zA-Z]{2,}))$/
      );
  };

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

  public validate(): boolean{
    if(this.registerModel.nombre == ''){
      this.errorMessage = "Error: El campo nombre es obligatorio"
      this.error = true;
      return false;
    }
    if(this.registerModel.apellido == ''){
      this.errorMessage = "Error: El campo apellido es obligatorio"
      this.error = true;
      return false;
    }
    if(this.registerModel.username == ''){
      this.errorMessage = "Error: El campo username es obligatorio"
      this.error = true;
      return false;
    }
    if(this.registerModel.email == ''){
      this.errorMessage = "Error: El campo email es obligatorio"
      this.error = true;
      return false;
    }
    if(this.registerModel.password == ''){
      this.errorMessage = "Error: El campo contraseña es obligatorio"
      this.error = true;
      return false;
    }
    if(this.registerModel.confirmPassword == ''){
      this.errorMessage = "Error: El campo confimacion de contraseña es obligatorio"
      this.error = true;
      return false;
    }
    if(this.validateEmail(this.registerModel.email) == null){
      this.errorMessage = "Error: El campo email no tiene el formato correcto"
      this.error = true;
      return false;
    }
    if(!this.validatePassword(this.registerModel.password)){
      return false;
    }
    if(this.registerModel.password != this.registerModel.confirmPassword){
      this.errorMessage = "Error: El campo de confirmar contraseña no es el mismo que el contraseña"
      this.error = true;
      return false;
    }
    return true;
  }

  onSubmit(){
    //Primero validamos los campos
    if(!this.validate()){ return; }

    //Otenemos el listado de usuarios
    let data = JSON.parse(localStorage.getItem("usuarios") || '{}');

    //Tenemos que iterar la lista
    for(let i = 0; i < data.length; i++){
      //Tenemos que verificar si existe el usuario
      let user = data[i];
      if(user.email == this.registerModel.email){
          //Ingresamos el usuario en el localstorage
          this.errorMessage = "Error: Ya existe un usuario con ese email"
          this.error = true;
          return;
      }
      if(user.username == this.registerModel.username){
        //Ingresamos el usuario en el localstorage
        this.errorMessage = "Error: Ya existe un usuario con ese username"
        this.error = true;
        return;
    }
    }
    //Agregamos el usuario
    data.push(this.registerModel);
    //Actualizamos la tabla de usuarios
    localStorage.setItem("usuarios", JSON.stringify(data));
    //Enviamos el email con la fecha y hora del registro
    let htmlEmail = `<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
    <html xmlns="http://www.w3.org/1999/xhtml" lang="EN">
      <head>
        <style type="text/css">
          @media screen {
            @font-face {
              font-family: "Lato";
              font-style: normal;
              font-weight: 400;
              src: local("Lato Regular"), local("Lato-Regular"), url(https://fonts.gstatic.com/s/lato/v11/qIIYRU-oROkIk8vfvxw6QvesZW2xOQ-xsNqO47m55DA.woff) format("woff");
            }
            body, html {
              margin: 0px;
              padding: 0px;
              -webkit-font-smoothing: antialiased;
              text-size-adjust: none;
              width: 100% !important;
              background: #F9F9FF;
              font-family: "Lato", "Lucida Grande", "Lucida Sans Unicode", Tahoma, Sans-Serif;
              word-break: break-word;
            }
            .contentMainTable {
              background: #FFFFFF;
              border: 1px solid #EEEEFF;
              margin-top: 98px;
              margin-bottom: 69px;
              margin-left: auto;
              margin-right: auto;
              width: 600px;
              height: 503px;
            }
            .ExternalClass, .ExternalClass p, .ExternalClass span, .ExternalClass font, .ExternalClass td, .ExternalClass div {
              line-height: 100%;
            }
            .ExternalClass {
              width: 100%;
            }
            .logoImage {
              margin-top: -50px;
              padding-bottom: 7px;
            }
            h1 {
              font-weight: bold;
              font-size: 30px;
              font-family: "Lato";
              letter-spacing: 0px;
              color: #25254E;
            }
            p {
              font-weight: 300;
              font-size: 14px;
              letter-spacing: 0px;
              color: #4D4D80;
            }
            .greyLine {
              border: 1px solid #CED7F7;
              width: 100%;
              margin-top: 32px
            }
            h2 {
              font-weight: bold;
              font-size: 15px;
              letter-spacing: 0px;
              color: #25254E;
            }
            h3 {
              font-weight: 300;
              font-size: 15px;
              letter-spacing: 0px;
              color: #4D4D80;
            }
            .footer {
              margin-top: 32px;
              margin-bottom: 20px;
              font-size: 11px;
              font-weight: 300px;
              color: #4D4D80;
            }
            .footerIcons img {
              margin-left: 11px;
              margin-right: 11px;
            }
            .blueButton {
              background: #8AA1EB;
              border-radius: 10px;
              padding: 17px 35px;
              border: none;
              color: #FFFFFF;
              font-size: 15px;
              margin-bottom: 32px;
              cursor: pointer;
            }
            .blueButton:focus {
              outline: none;
              border: 2px solid #5457FF;
              padding: 15px 33px;
            }
            @media only screen and (max-width: 480px) {
              table, table tr td, table td {
                width: 100%;
              }
              .contentMainTable {
                width: 100%;
                border: none;
              }
              body, html {
                background: #FFFFFF;
              }
              h1 {
                font-size: 24px;
              }
            }
          }
        </style>
        <title>
        </title>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
      </head>
      <body style="padding:0; margin: 0;">
        <table border="0" cellpadding="0" cellspacing="0" height="100%" width="100%" id="body_table">
          <tbody>
            <tr>
              <td align="center" valign="top"> 
                <table border="0" cellpadding="20" cellspacing="0" width="100%" class="contentMainTable">
                  <tbody>
                    <tr>
                      <td align="center" valign="top"> 
                        <table border="0" cellpadding="20" cellspacing="0" width="100%" id="content">
                          <tbody>
                            <tr>
                              <td align="center" valign="top">
                                <span class="isDesktop">
                                  <h1 style="margin-bottom: 32px">
                                    Solicitud de cuenta de FuBox Correcta!
                                  </h1>
                                  <p style="margin-top:0px">Podras tener acceso a todas las nuevas opciones de almacenamiento en la nube.
                                  Fecha y Hora del registro: ` + new Date() + `
                                  </p></span>
                                <div class="greyLine">
                                </div>
                              </td>
                            </tr>
                          </tbody>
                        </table>
                      </td>
                    </tr>
                  </tbody>
                </table>
              </td>
            </tr>
          </tbody>
        </table>
      </body>
    </html>`
    //Ahora mandamos el email
    Email.send({
      Host : 'smtp.elasticemail.com',
      Username : 'angel.arteaga01.aa@gmail.com',
      Password : '60370CBD05FC5D40D1691ED8340452947BEF',
      To : this.registerModel.email,
      From : 'angel.arteaga01.aa@gmail.com',
      Subject : 'Confirmacion de Solicitud - FuBox',
      Body : htmlEmail
    }).then( console.log("Pues todo bien bro jaja") );
    //Redireccionamos a la pagina de confirmacion
    this.router.navigate(['/registerConfirm']);
  }

}
