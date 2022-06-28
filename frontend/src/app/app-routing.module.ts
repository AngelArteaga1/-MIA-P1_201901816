import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { TreeComponent } from './components/tree/tree.component';
import { LoginComponent } from './components/login/login.component';
import { RegisterComponent } from './components/register/register.component';
import { UsersComponent } from './components/users/users.component';
import { RegisterConfirmComponent } from './components/register-confirm/register-confirm.component';
import { RecoverPasswordComponent } from './components/recover-password/recover-password.component';
import { RecoverConfirmComponent } from './components/recover-confirm/recover-confirm.component';
import { ProfileComponent } from './components/profile/profile.component';

const routes: Routes = [
  { path: '', component: TreeComponent },
  { path: 'login', component: LoginComponent },
  { path: 'register', component: RegisterComponent },
  { path: 'users', component: UsersComponent },
  { path: 'registerConfirm', component: RegisterConfirmComponent },
  { path: 'recoverPassword/:id', component: RecoverPasswordComponent },
  { path: 'recoverConfirm', component: RecoverConfirmComponent },
  { path: 'profile/:username', component: ProfileComponent }
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }
