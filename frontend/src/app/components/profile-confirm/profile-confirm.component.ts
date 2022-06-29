import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';

@Component({
  selector: 'app-profile-confirm',
  templateUrl: './profile-confirm.component.html',
  styleUrls: ['./profile-confirm.component.css']
})
export class ProfileConfirmComponent implements OnInit {

  constructor(private router: Router) { }

  ngOnInit(): void {
    //Verificando si ya esta logeado
    if(localStorage.getItem('logeado') == 'false'){this.router.navigate(['/login']); return;}
  }

}
